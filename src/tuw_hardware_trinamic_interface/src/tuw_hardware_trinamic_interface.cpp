#include "tuw_hardware_trinamic_interface/tuw_hardware_trinamic_interface.hpp"
#include "hardware_interface/lexical_casts.hpp"
#include <cerrno>
#include <cmath>

namespace tuw_hardware_trinamic_interface {
    hardware_interface::CallbackReturn TrinamicInterface::on_init(const hardware_interface::HardwareComponentInterfaceParams &params) {
        if(hardware_interface::SystemInterface::on_init(params) != hardware_interface::CallbackReturn::SUCCESS) {
            hardware_interface::CallbackReturn::ERROR;
        }

        // setup hardware interface (aka this class)

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

        // 3. fast mode (fast_mode = true) or accurate mode (fast_mode = false)
        //    fast mode: the last received reply's value (which is the last sent value) will be used as value for the state interfaces
        //    accurate mode: the value for the state interfaces is the hardware's actual velocity
        sys_param = get_hardware_info().hardware_parameters.find("fast_mode");
        if(sys_param != get_hardware_info().hardware_parameters.end()) {
            fast_mode = hardware_interface::parse_bool(sys_param->second);
        }

        // get the parameters for the different joints (in this case: wheels), which is the respective serial_port
        if(test_mode) {
            return hardware_interface::CallbackReturn::SUCCESS;
        }

        for(auto joint : get_hardware_info().joints) {
            if(joint.parameters.find("serial_port") == joint.parameters.end()) {
                RCLCPP_ERROR(this->get_node()->get_logger(), "Joint \"%s\" has no specified serial port!", joint.name.c_str());
                return hardware_interface::CallbackReturn::ERROR;
            }
        }
        //auto joint_params = get_hardware_info().joints.
        return hardware_interface::CallbackReturn::SUCCESS;
    }


    hardware_interface::CallbackReturn TrinamicInterface::on_configure(const rclcpp_lifecycle::State &previous_state) {
        // setup communication with robot hardware
        if(test_mode) {
            for (int i = 0; i < get_hardware_info().joints.size(); i++) {
                tmcm1640::TMCM1640Connection wheel(test_mode, get_hardware_info().joints[i].name, true);
                wheels.push_back(wheel);
            }
        } else {

            for (auto joint : get_hardware_info().joints) {
                try {
                    tmcm1640::TMCM1640Connection wheel(joint.parameters.find("serial_port")->second, joint.name);
                    wheels.push_back(wheel);
                } catch(std::exception &e) {
                    RCLCPP_ERROR(this->get_node()->get_logger(), "ERROR INITIALIZING THE SERIAL CONNECTION OF JOINT \"%s\": %s", joint.name.c_str(), e.what());
                    return hardware_interface::CallbackReturn::ERROR;
                }
            }
        }

        // initial values for state and command interfaces
        for (auto joint : get_hardware_info().joints) {
            for(auto cmd_int : joint.command_interfaces) {
                set_command(joint.name + "/" + cmd_int.name, 0.0);
            }
            for(auto state_int : joint.state_interfaces) {
                set_state(joint.name + "/" + state_int.name, 0.0);
            }
        }

        // write initial values of MST to wheels and get correct value
        for(auto wheel : wheels) {
            int32_t val = wheel.communicate(tmcm1640::tmcm1640_cmd::MST);
            if(val != 0) {
                RCLCPP_ERROR(this->get_node()->get_logger(), "Couldn't initialize wheel %s: Got value %d instead of 0", wheel.get_name(), val);
                return hardware_interface::CallbackReturn::ERROR;
            }
        }

        return hardware_interface::CallbackReturn::SUCCESS;
    }


    hardware_interface::return_type TrinamicInterface::read(const rclcpp::Time &time, const rclcpp::Duration &period) {
        // write values from hardware to state interfaces
        // fast mode: Assumption: the value is the same value as was received by the last reply
        /*for(auto wheel : wheels) {
            set_state(wheel.name + "/velocity", wheel_vel_to_mps(wheel.get_value()));
            }*/

        // accurate mode: always getting the newest value from the hardware
        for(auto wheel : wheels) {
            // in normal mode the value sent to the hardware doesn't matter, in test mode the value will be mirrored
            int32_t val = wheel.communicate(tmcm1640::tmcm1640_cmd::GAP, static_cast<int>(tmcm1640::tmcm1640_axis_params::ACTUAL_VEL), wheel.get_value());
            set_state(wheel.get_name() + "/velocity", wheel_vel_to_mps(val));
        }
        return hardware_interface::return_type::OK;
    }

    hardware_interface::return_type TrinamicInterface::write(const rclcpp::Time &time, const rclcpp::Duration &period) {
        // TODO write values from command interfaces to hardware

        for(auto wheel : wheels) {
            double cmd = get_command(wheel.get_name() + "/velocity");
            int32_t val;
            if (cmd == 0.0) {
                val = wheel.communicate(tmcm1640::tmcm1640_cmd::MST);
            } else {
                val = wheel.communicate(tmcm1640::tmcm1640_cmd::ROR, cmd_vel_to_rpm(cmd));
            }

            set_state(wheel.get_name() + "/velocity", wheel_vel_to_mps(val));
        }

        return hardware_interface::return_type::OK;
    }

    int32_t TrinamicInterface::cmd_vel_to_rpm(double vel) {
        return vel*60/(M_PI*wheel_diameter);
    }

    double TrinamicInterface::wheel_vel_to_mps(int32_t vel) {
        return vel*M_PI*wheel_diameter/60.;
    }

}

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(tuw_hardware_trinamic_interface::TrinamicInterface, hardware_interface::SystemInterface)
