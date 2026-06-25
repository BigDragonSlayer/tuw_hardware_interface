#include <rclcpp/rclcpp.hpp>
#include <functional>
#include <string>
#include <chrono>
#include <thread>
#include "tuw_hardware_trinamic_interface/tuw_hardware_trinamic_definitions.hpp"
#include "tuw_hardware_trinamic_interface/tuw_hardware_trinamic_connection.hpp"
#include <cerrno>
#include <memory>
#include <vector>

using namespace std::chrono_literals;

class TestNode : public rclcpp::Node {
    public:
    TestNode()
    : Node("trinamic_test_node") {
        this->declare_parameter("serial_port", "");

        auto timer_callback = [this]() {
            serial_port = this->get_parameter("serial_port").as_string();
            RCLCPP_INFO(this->get_logger(), "Serial port: %s", serial_port.c_str());

            std::vector<rclcpp::Parameter> new_parameters{rclcpp::Parameter("serial_port", "")};
            this->set_parameters(new_parameters);

            test_ptr();
        };

        timer = this->create_wall_timer(1000ms, timer_callback);
    }

    private:
        std::string serial_port = "";
        bool tested = false;
        rclcpp::TimerBase::SharedPtr timer;

        void test_ptr() {
            if(serial_port.compare("") == 0) {
                return;
            }

            RCLCPP_INFO(this->get_logger(), "in test_ptr");

            std::vector<std::shared_ptr<tmcm1640::TMCM1640Connection>> wheels;

            {
            std::shared_ptr<tmcm1640::TMCM1640Connection> wheel = std::make_shared<tmcm1640::TMCM1640Connection>(serial_port, "test_wheel");
            wheels.push_back(wheel);
            }

            wheels[0]->communicate(tmcm1640::tmcm1640_cmd::ROR, 100);
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }

        void test_code() {
            if(serial_port.compare("") == 0 || tested) {
                return;
            }

            RCLCPP_INFO(this->get_logger(), "in test_code");
            tested = true;

            try{
                tmcm1640::TMCM1640Connection wheel(serial_port, "test_wheel");

                try {
                    wheel.communicate(tmcm1640::tmcm1640_cmd::ROR, 100);
                } catch (std::exception &e) {
                    RCLCPP_ERROR(this->get_logger(), "COMMUNICATION ERROR: %s", e.what());
                }

                //std::array<std::uint8_t, 9> reply = wheel.get_whole_reply();

                /*for(uint8_t& el : reply) {
                    RCLCPP_INFO(this->get_logger(), "%d", el);
                }*/

                std::this_thread::sleep_for(std::chrono::seconds(3));

                try {
                    wheel.communicate(tmcm1640::tmcm1640_cmd::MST);
                } catch (std::exception &e) {
                    RCLCPP_ERROR(this->get_logger(), "COMMUNICATION ERROR: %s", e.what());
                }

                //reply = wheel.get_whole_reply();

                /*for(uint8_t& el : reply) {
                    RCLCPP_INFO(this->get_logger(), "%d", el);
                }*/

            } catch (std::exception &e) {
                RCLCPP_ERROR(this->get_logger(), "ERROR INITIALIZING THE SERIAL CONNECTION: %s", e.what());
                return;
            }

        }
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TestNode>());
    rclcpp::shutdown();
    return 0;
}
