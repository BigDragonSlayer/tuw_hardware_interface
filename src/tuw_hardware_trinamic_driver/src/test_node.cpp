#include <rclcpp/rclcpp.hpp>
#include <functional>
#include <string>
#include <chrono>
#include <thread>
#include "tuw_hardware_trinamic_driver/tuw_hardware_trinamic_definitions.hpp"
#include "tuw_hardware_trinamic_driver/tuw_hardware_trinamic_connection.hpp"

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

            test_code();
        };

        timer = this->create_wall_timer(1000ms, timer_callback);
    }

    private:
        std::string serial_port = "";
        bool tested = false;
        rclcpp::TimerBase::SharedPtr timer;

        void test_code() {
            if(serial_port.compare("") == 0 || tested) {
                return;
            }

            tmcm1640::TMCM1640Connection wheel(serial_port);

            wheel.communicate(tmcm1640::tmcm1640_cmd::ROR, 100);

            std::this_thread::sleep_for(std::chrono::seconds(3));

            wheel.communicate(tmcm1640::tmcm1640_cmd::MST);

            tested = true;
        }
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TestNode>());
    rclcpp::shutdown();
    return 0;
}
