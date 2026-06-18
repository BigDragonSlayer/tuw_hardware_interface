// Copyright 2022 Eugen Kaltenegger

#ifndef DIP_WS_TRINAMIC_HARDWARE_INTERFACE_H
#define DIP_WS_TRINAMIC_HARDWARE_INTERFACE_H

#include "tuw_hardware_interface_template/generic_hardware_interface.h"
#include "tuw_hardware_interface_trinamic/description/trinamic_joint_description.h"

#include <pluginlib/class_list_macros.hpp>
#include <hardware_interface/robot_hw.h>

namespace tuw_hardware_interface
{
class TrinamicHardwareInterface : public GenericHardwareInterface
{
public:
  bool init(ros::NodeHandle &basic_node_handle, ros::NodeHandle &hardware_node_handle) override;
private:
  bool initJoints(std::list<TrinamicJointDescription> joint_descriptions);
  bool initJoint(TrinamicJointDescription joint_description);
};
}  // namespace tuw_hardware_interface

#endif //DIP_WS_TRINAMIC_HARDWARE_INTERFACE_H
