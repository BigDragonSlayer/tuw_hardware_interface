#ifndef TUW_HARDWARE_TRINAMIC_INTERFACE_H_
#define TUW_HARDWARE_TRINAMIC_INTERFACE_H_
#include "hardware_interface/system_interface.hpp"
#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include "tuw_hardware_trinamic_driver/tuw_hardware_trinamic_connection.hpp"
#include <vector>

namespace tuw_hardware_trinamic_interface {

    class TrinamicInterface : public hardware_interface::SystemInterface {
        public:
            // overriding a HardwareComponentInterface method
            hardware_interface::CallbackReturn on_init(const hardware_interface::HardwareComponentInterfaceParams &params) override;
            // overriding a LifecycleNode method
            hardware_interface::CallbackReturn on_configure(const rclcpp_lifecycle::State &previous_state) override;
            // overriding a HardwareComponentInterface method
            hardware_interface::return_type read(const rclcpp::Time &time, const rclcpp::Duration &period) override;
            // overriding a HardwareComponentInterface method
            hardware_interface::return_type write(const rclcpp::Time &time, const rclcpp::Duration &period) override;


        private:
            // TODO stuff i need

            // the hardware
            std::vector<std::string> serial_ports;
            std::vector<std::string> test_mode;
            std::vector<tmcm1640::TMCM1640Connection> wheels;
    };
}


#endif // TUW_HARDWARE_TRINAMIC_INTERFACE_H_
