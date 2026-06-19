#ifndef TUW_HARDWARE_TRINAMIC_CONNECTION_H_
#define TUW_HARDWARE_TRINAMIC_CONNECTION_H_

#include "tuw_hardware_trinamic_driver/tuw_hardware_trinamic_definitions.hpp"

// for the serial communication
#include <fcntl.h>    // file controls like O_RDWR
#include <errno.h>    // Error integer and stderror() function
#include <termios.h>  // Contains POSIX terminal control definitions
#include <unistd.h>   // write(), read(), close()

#include <cstdint>
#include <array>
#include <string>

// möglicherweise cstdint?
namespace tmcm1640 {
    class TMCM1640Connection {
        public:
            TMCM1640Connection(std::string port);
            ~TMCM1640Connection();

            int32_t communicate(tmcm1640_cmd cmd);
            int32_t communicate(tmcm1640_cmd cmd, int value);
            int32_t communicate(tmcm1640_cmd cmd, int type, int value);
            std::array<std::uint8_t, 9> get_whole_reply();
            int32_t get_value();

        private:
            bool send_and_receive();
            std::uint8_t calc_checksum(std::array<std::uint8_t, 9> msg);
            uint8_t check_reply();
            void set_value();

            std::array<std::uint8_t, 9> command_message;
            std::array<std::uint8_t, 9> reply_message;
            int32_t value;
            int serial_port;

    };
}

#endif // TUW_HARDWARE_TRINAMIC_CONNECTION_H_
