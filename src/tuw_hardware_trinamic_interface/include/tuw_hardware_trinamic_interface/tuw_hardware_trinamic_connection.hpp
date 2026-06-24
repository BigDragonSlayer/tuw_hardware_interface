#ifndef TUW_HARDWARE_TRINAMIC_CONNECTION_H_
#define TUW_HARDWARE_TRINAMIC_CONNECTION_H_

#include "tuw_hardware_trinamic_interface/tuw_hardware_trinamic_definitions.hpp"

// for the serial communication
#include <fcntl.h>    // file controls like O_RDWR
#include <errno.h>    // Error integer and stderror() function
#include <termios.h>  // Contains POSIX terminal control definitions
#include <unistd.h>   // write(), read(), close()

#include <cstdint>
#include <array>
#include <string>

namespace tmcm1640 {
    class TMCM1640Connection {
        public:
            TMCM1640Connection(std::string port, std::string name);
            TMCM1640Connection(bool test_mode, std::string name, bool test_with_correct_checksum);
            ~TMCM1640Connection();

            int32_t communicate(tmcm1640_cmd cmd);
            int32_t communicate(tmcm1640_cmd cmd, int32_t value);
            int32_t communicate(tmcm1640_cmd cmd, int type, int32_t value);
            std::array<std::uint8_t, 9> get_whole_reply() {return reply_message; }
            int32_t get_value() { return value; }
            bool get_test_mode() { return test_mode; }
            bool get_checksum_mode() { return correct_checksum; }
            void set_checksum_mode(bool test_with_correct_checksum) { correct_checksum = test_with_correct_checksum; }
            std::string get_name() { return name; }

        private:
            void init_cmd_msg();
            bool send_and_receive();
            bool test_with_correct_checksum();
            bool test_with_wrong_checksum();
            std::uint8_t calc_checksum(std::array<std::uint8_t, 9> msg);
            uint8_t check_reply();
            void set_value();

            std::array<std::uint8_t, 9> command_message;
            std::array<std::uint8_t, 9> reply_message;
            int32_t value;
            int serial_port;
            bool test_mode = false;
            bool correct_checksum;
            std::string name;
    };
}

#endif // TUW_HARDWARE_TRINAMIC_CONNECTION_H_
