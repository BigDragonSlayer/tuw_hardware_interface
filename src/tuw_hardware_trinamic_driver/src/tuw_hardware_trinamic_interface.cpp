#include "tuw_hardware_trinamic_driver/tuw_hardware_trinamic_interface.hpp"

namespace tuw_hardware_trinamic_interface {
    hardware_interface::CallbackReturn TrinamicInterface::on_init(const hardware_interface::HardwareComponentInterfaceParams &params) {
        if(hardware_interface::SystemInterface::on_init(params) != hardware_interface::CallbackReturn::SUCCESS) {
            hardware_interface::CallbackReturn::ERROR;
        }


        // TODO setup hardware interface (aka this class)
        return hardware_interface::CallbackReturn::SUCCESS;
    }


    hardware_interface::CallbackReturn TrinamicInterface::on_configure(const rclcpp_lifecycle::State &previous_state) {
        // TODO setup communication with robot hardware
        // initial values for state and command interfaces
        return hardware_interface::CallbackReturn::SUCCESS;
    }


    hardware_interface::return_type TrinamicInterface::read(const rclcpp::Time &time, const rclcpp::Duration &period) {
        // TODO write values from hardware to state interfaces

        return hardware_interface::return_type::OK;
    }

    hardware_interface::return_type TrinamicInterface::write(const rclcpp::Time &time, const rclcpp::Duration &period) {
        // TODO write values from command interfaces to hardware
        //
        return hardware_interface::return_type::OK;
    }

}

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(tuw_hardware_trinamic_interface::TrinamicInterface, hardware_interface::SystemInterface)
