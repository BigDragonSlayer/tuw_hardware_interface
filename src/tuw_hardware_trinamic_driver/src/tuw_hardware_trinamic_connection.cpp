#include "tuw_hardware_trinamic_connection.hpp"

namespace tmcm1640 {
    TMCM1640Connection::TMCM1640Connection(std::string_port) {
        // initialize the serial connection
        // initialize the command_message
        command_message[tmcm1640_cmd_format::TARGET] = TARGET;
        command_message[tmcm1640_cmd_format::TYPE] = TYPE_DEFAULT;
        command_message[tmcm1640_cmd_format::MOT_BANK] = MOTOR_NUMBER;
        command_message[tmcm1640_cmd_format::VALUE_MSB] = VALUE_DEFAULT; // TODO funktioniert das so?
    }

    TMCM1640Connection::~TMCM1640Connection() {
        // TODO implement
        // close serial connection
    }

    int TMCM1640Connection::communicate(tmcm1640_cmd cmd) {
        return communicate(cmd, TYPE_DEFAULT, VALUE_DEFAULT);
    }

    int TMCM1640Connection::communicate(tmcm1640_cmd cmd, int value) {
        return communicate(cmd, TYPE_DEFAULT, value);
    }

    int TMCM1640Connection::communicate(tmcm1640_cmd cmd, int type, int value) {
        // TODO implement
        // update the command_message
        command_message[tmcm1640_cmd_format::CMD] = cmd;
        command_message[tmcm1640_cmd_format::TYPE] = type;
        command_message[tmcm1640_cmd_format::VALUE_MSB] = value;

        // calculate the checksum
        command_message[tmcm1640_cmd_format::CHECKSUM] = calc_checksum();

        // send the message and receive the tmcm1640's answer
        if (!send_and_receive()) {
            return ERROR; // TODO implement properly
        }

        // check the reply
        if (!(check_reply() == tmcm1640_status_codes::OK) && !(check_reply() == tmcm1640_status_codes::CMD_LOADED)) { // TODO do I need CONFIG_LOCKED?
            return ERROR; // TODO implement properly
        }

        // return the value
        return value;
    }

    std::array<std::uint8_t, 9> TMCM1640Connection::get_whole_reply() {
        return reply_message;
    }

    int TMCM1640Connection::get_value() {
        return value;
    }

    tmcm1640_status_codes TMCM1640Connection::send_and_receive() {
        // TODO implement
        // send command_message using the serial connection
        // receive the reply sent on the serial connection and store it into reply_message
        // TODO return whether it worked (how???? and with which type????)
    }

    std::uint8_t TMCM1640Connection::calc_checksum(std::array<std::uint8_t, 9> msg) {
        // the algorithm from the tmcm1640's datasheet
        std::uint8_t checksum = 0;

        for (int i = 0; i < 9; i++) {
            checksum += msg[i];
        }

        return checksum;
    }

    tmcm1640_status_codes TMCM1640Connection::check_reply() {
        // TODO implement
        // check whether the reply has the correct checksum
        if(!(calc_checksum == reply_message[tmcm1640_reply_format::CHECKSUM])) {
            return ERROR;
        }

        // if yes, return the status code
        return reply_message[tmcm1640_reply_format::STATUS];
    }

    void TMCM1640Connection::set_value() {
        value = reply_message[tmcm1640_reply_format::VALUE_MSB]; // TODO set value size to 4 bytes exactly
        // TODO kann man sicher vernünftiger schreiben
        // TODO vllt nicht mit Methode sondern mit pointer auf VALUE_MSB?
    }
}
