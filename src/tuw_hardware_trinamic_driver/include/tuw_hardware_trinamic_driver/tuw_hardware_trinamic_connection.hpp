#ifndef TUW_HARDWARE_TRINAMIC_CONNECTION_H_
#define TUW_HARDWARE_TRINAMIC_CONNECTION_H_

#include "tuw_hardware_trinamic_definitions.hpp"

namespace tmcm1640 {
    class TMCM1640Connection {
        public:
            TMCM1640Connection(std::string port);
            ~TMCM1640Connection();

            int communicate(tmcm1640_cmd cmd);
            int communicate(tmcm1640_cmd cmd, int value);
            int communicate(tmcm1640_cmd cmd, int type, int value);
            std::array<std::uint8_t, 9> get_whole_reply();
            int get_value();

        private:
            tmcm1640_status_codes send_and_receive();
            std::uint8_t calc_checksum();
            tmcm1640_status_codes check_reply();
            void set_value();

            std::array<std::uint8_t, 9> command_message;
            std::array<std::uint8_t, 9> reply_message;
            int value;

    };
}

#endif // TUW_HARDWARE_TRINAMIC_CONNECTION_H_
