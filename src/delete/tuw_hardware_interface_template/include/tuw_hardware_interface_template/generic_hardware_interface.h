// Copyright 2022 Eugen Kaltenegger

#ifndef TUW_HARDWARE_INTERFACE_TEMPLATE_GENERIC_HARDWARE_INTERFACE_H
#define TUW_HARDWARE_INTERFACE_TEMPLATE_GENERIC_HARDWARE_INTERFACE_H

#include <list>
#include <memory>
#include <string>
#include <vector>

#include <hardware_interface/robot_hw.h>

#include <tuw_hardware_interface_template/generic_joint.h>

using tuw_hardware_interface::GenericJoint;
using tuw_hardware_interface::GenericJointDescription;

namespace tuw_hardware_interface
{
class GenericHardwareInterface : public hardware_interface::RobotHW
{
public:
  bool init(ros::NodeHandle &basic_node_handle, ros::NodeHandle &hardware_node_handle) override;
  void read(const ros::Time &time, const ros::Duration &period) override;
  void write(const ros::Time &time, const ros::Duration &period) override;
  void doSwitch(const std::list<hardware_interface::ControllerInfo>& start_list,
                const std::list<hardware_interface::ControllerInfo>& stop_list) override;
protected:
  virtual bool initJoints(std::list<GenericJointDescription> joint_descriptions);
  virtual bool initJoint(GenericJointDescription joint_description);
  std::shared_ptr<GenericJoint> findJoint(const std::string& name);

  static std::string setup_parameter_;
  std::vector<std::shared_ptr<GenericJoint>> joints_;
  hardware_interface::JointStateInterface joint_state_interface;
  hardware_interface::PositionJointInterface joint_position_interface;
  hardware_interface::VelocityJointInterface joint_velocity_interface;
  hardware_interface::EffortJointInterface joint_effort_interface;
};
}  // namespace tuw_hardware_interface

#endif  // TUW_HARDWARE_INTERFACE_TEMPLATE_GENERIC_HARDWARE_INTERFACE_H
