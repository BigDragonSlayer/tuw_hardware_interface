#include "tuw_hardware_trinamic_interface/tuw_hardware_trinamic_connection.hpp"
#include "tuw_hardware_trinamic_interface/tuw_hardware_trinamic_definitions.hpp"
#include <cerrno>
#include <boost/format.hpp>
#include <rclcpp/rclcpp.hpp>

namespace tmcm1640 {
    TMCM1640Connection::TMCM1640Connection(std::string port) {
        // initialize the serial connection
        serial_port = open(port.c_str(), O_RDWR);

        // check for errors
        if (serial_port < 0) {
            std::error_code error_code(errno, std::generic_category());
            std::string error_message = (boost::format("Error %i from open: %s\n") % errno % strerror(errno)).str();
            throw std::system_error(error_code, error_message.c_str());
        }

        struct termios tty;

        if(tcgetattr(serial_port, &tty) != 0) {
            close(serial_port);
            std::error_code error_code(errno, std::generic_category());
            std::string error_message = (boost::format("Error %i from tcgetattr: %s\n") % errno % strerror(errno)).str();
            throw std::system_error(error_code, error_message.c_str());
        }

        // set the flags
        // clear parity bit
        tty.c_cflag &= ~PARENB;
        // clear stop field
        tty.c_cflag &= ~CSTOPB;
        // clear size bit
        tty.c_cflag &= ~CSIZE;
        // set size bit
        tty.c_cflag |= CS8;
        // disable RTS/CTS hardware flow control
        tty.c_cflag &= CRTSCTS;
        // turn on read (and ignore control lines)
        tty.c_cflag |= CREAD | CLOCAL;

        tty.c_lflag &= ~ICANON;
        // disable echo
        tty.c_lflag &= ~ECHO;
        // disable erasure
        tty.c_lflag &= ~ECHOE;
        // disable new-line echo
        tty.c_lflag &= ~ECHONL;
        // disable interpretation of INTR, QUIT ans SUSP
        tty.c_lflag &= ~ISIG;
        // disable s/w flow control
        tty.c_iflag &= ~(IXON | IXOFF | IXANY);
        // disable special handling of received bytes
        tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

        // prevent special interpretation of output bytes
        tty.c_oflag &= ~OPOST;
        // prevent conversion of new-line to carriage return
        tty.c_oflag &= ~ONLCR;

        // set wait for 9 bytes
        tty.c_cc[VTIME] = 0;
        tty.c_cc[VMIN] = 9;

        cfsetispeed(&tty, B9600);
        cfsetospeed(&tty, B9600);

        if(tcsetattr(serial_port, TCSANOW, &tty) != 0) {
            close(serial_port);
            std::error_code error_code(errno, std::generic_category());
            std::string error_message = (boost::format("Error %i from tcsetattr: %s\n") % errno % strerror(errno)).str();
            throw std::system_error(error_code, error_message.c_str());
        }

        // initalize variables
        init_cmd_msg();
    }

    TMCM1640Connection::TMCM1640Connection(bool test_mode, bool test_with_correct_checksum) {
        this->test_mode = test_mode;
        this->correct_checksum = test_with_correct_checksum;
        init_cmd_msg();
    }

    void TMCM1640Connection::init_cmd_msg() {
        // initialize the command_message
        command_message[static_cast<int>(tmcm1640_cmd_format::CMD_TARGET)] = TARGET;
        command_message[static_cast<int>(tmcm1640_cmd_format::TYPE)] = TYPE_DEFAULT;
        command_message[static_cast<int>(tmcm1640_cmd_format::MOT_BANK)] = MOTOR_NUMBER;
        command_message[static_cast<int>(tmcm1640_cmd_format::VALUE3)] = VALUE_DEFAULT;
        command_message[static_cast<int>(tmcm1640_cmd_format::VALUE2)] = VALUE_DEFAULT;
        command_message[static_cast<int>(tmcm1640_cmd_format::VALUE1)] = VALUE_DEFAULT;
        command_message[static_cast<int>(tmcm1640_cmd_format::VALUE0)] = VALUE_DEFAULT;
    }

    TMCM1640Connection::~TMCM1640Connection() {
        // close serial connection
        if (!test_mode) {
            communicate(tmcm1640_cmd::MST);
            close(serial_port);
        }
    }

    int32_t TMCM1640Connection::communicate(tmcm1640_cmd cmd) {
        return communicate(cmd, TYPE_DEFAULT, VALUE_DEFAULT);
    }

    int32_t TMCM1640Connection::communicate(tmcm1640_cmd cmd, int value) {
        return communicate(cmd, TYPE_DEFAULT, value);
    }

    int32_t TMCM1640Connection::communicate(tmcm1640_cmd cmd, int type, int value) {
        // update the command_message
        command_message[static_cast<int>(tmcm1640_cmd_format::CMD)] = static_cast<uint8_t>(cmd);
        command_message[static_cast<int>(tmcm1640_cmd_format::TYPE)] = type;
        command_message[static_cast<int>(tmcm1640_cmd_format::VALUE3)] = value >> 24;
        command_message[static_cast<int>(tmcm1640_cmd_format::VALUE2)] = value >> 16;
        command_message[static_cast<int>(tmcm1640_cmd_format::VALUE1)] = value >> 8;
        command_message[static_cast<int>(tmcm1640_cmd_format::VALUE0)] = value;

        // calculate the checksum
        command_message[static_cast<int>(tmcm1640_cmd_format::CHECKSUM)] = calc_checksum(command_message);

        if(test_mode) {
            if(correct_checksum) {
                test_with_correct_checksum();
            } else {
                test_with_wrong_checksum();
            }
        } else {
            // send the message and receive the tmcm1640's answer
            if (!send_and_receive()) {
                std::error_code error_code(errno, std::generic_category());
                std::string error_message = "Communication error: could not receive reply";
                throw std::system_error(error_code, error_message.c_str());
            }
        }

        // check the reply
        uint8_t reply_status = check_reply();
        if (!(reply_status == static_cast<uint8_t>(tmcm1640_status_codes::OK)) && !(reply_status == static_cast<uint8_t>(tmcm1640_status_codes::CMD_LOADED))) { // TODO do I need CONFIG_LOCKED?
            std::error_code error_code(errno, std::generic_category());
            std::string error_message = "Reply wrong. Status: " + std::to_string(reply_status);
            throw std::system_error(error_code, error_message.c_str());
        }

        // return the value
        return value;
    }

    bool TMCM1640Connection::send_and_receive() {
        // send command_message using the serial connection
        write(serial_port, command_message.data(), 9);

        // receive the reply sent on the serial connection and store it into reply_message
        int num_bytes = read(serial_port, reply_message.data(), 9);

        if(num_bytes < 0) {
            return false;
        }

        return true;
    }

    bool TMCM1640Connection::test_with_correct_checksum() {
        reply_message = command_message; // I love the array class!
        return true; // just copying all elements of an array to another can't fail really
    }

    bool TMCM1640Connection::test_with_wrong_checksum() {
        reply_message = command_message;
        reply_message[static_cast<int>(tmcm1640_reply_format::CHECKSUM)] += 1;
        return true; // the same as above, and changing the checksum very much shouldn't be able to fail either
    }

    std::uint8_t TMCM1640Connection::calc_checksum(std::array<std::uint8_t, 9> msg) {
        // the algorithm from the tmcm1640's datasheet
        std::uint8_t checksum = 0;

        for (int i = 0; i < 8; i++) {
            checksum += msg[i];
        }

        return checksum;
    }

    uint8_t TMCM1640Connection::check_reply() {
        // check whether the reply has the correct checksum
        if(!(calc_checksum(reply_message) == reply_message[static_cast<int>(tmcm1640_reply_format::CHECKSUM)])) {
            return 7;         // Reply has wrong checksum
        }

        set_value();

        // if yes, return the status code
        return reply_message[static_cast<int>(tmcm1640_reply_format::STATUS)];
    }

    void TMCM1640Connection::set_value() {
        value = (reply_message[static_cast<int>(tmcm1640_reply_format::VALUE3)] << 24 |
                 reply_message[static_cast<int>(tmcm1640_reply_format::VALUE2)] << 16 |
                 reply_message[static_cast<int>(tmcm1640_reply_format::VALUE1)] << 8  |
                 reply_message[static_cast<int>(tmcm1640_reply_format::VALUE0)]);
    }
}
