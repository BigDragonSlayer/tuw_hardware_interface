#include "tuw_hardware_trinamic_interface/tuw_hardware_trinamic_interface.hpp"
#include "hardware_interface/lexical_casts.hpp"

namespace tuw_hardware_trinamic_interface {
    hardware_interface::CallbackReturn TrinamicInterface::on_init(const hardware_interface::HardwareComponentInterfaceParams &params) {
        if(hardware_interface::SystemInterface::on_init(params) != hardware_interface::CallbackReturn::SUCCESS) {
            hardware_interface::CallbackReturn::ERROR;
        }

        // TODO setup hardware interface (aka this class)

        // get the parameters for the plugin, which are
        // 1. whether the system is in TestMode (aka without real Hardware): test_mode: bool
        auto sys_param = get_hardware_info().hardware_parameters.find("test_mode");
        if(sys_param != get_hardware_info().hardware_parameters.end()) {
            test_mode = hardware_interface::parse_bool(sys_param->second);
        }

        // 2. the wheel diameter of the wheels (assumption: the diameter is the same for all wheels)
        sys_param = get_hardware_info().hardware_parameters.find("wheel_diameter");
        if(sys_param != get_hardware_info().hardware_parameters.end()) {
            wheel_diameter = hardware_interface::stod(sys_param->second);
        }

        // TODO get the parameters for the different joints (in this case: wheels), which is the respective serial_port
        for(auto joint : get_hardware_info().joints) {
            RCLCPP_INFO(this->get_node()->get_logger(), "length of parameters: %d", joint.parameters.size());
        }
        //auto joint_params = get_hardware_info().joints.
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
