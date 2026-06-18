// Copyright 2022 Eugen Kaltenegger

#ifndef TUW_HARDWARE_INTERFACE_DYNAMIXEL_DYNAMIXEL_HARDWARE_INTERFACE_H
#define TUW_HARDWARE_INTERFACE_DYNAMIXEL_DYNAMIXEL_HARDWARE_INTERFACE_H

#include <tuw_hardware_interface_template/generic_hardware_interface.h>

#include <list>

#include <pluginlib/class_list_macros.hpp>

namespace tuw_hardware_interface
{
class DynamixelJointDescription;
class DynamixelHardwareInterface : public GenericHardwareInterface
{
public:
  bool init(ros::NodeHandle &basic_node_handle, ros::NodeHandle &hardware_node_handle) override;
private:
  bool initJoints(std::list<DynamixelJointDescription> joint_descriptions);
  bool initJoint(DynamixelJointDescription joint_description);
};
}  // namespace tuw_hardware_interface

#endif  // TUW_HARDWARE_INTERFACE_DYNAMIXEL_DYNAMIXEL_HARDWARE_INTERFACE_H
