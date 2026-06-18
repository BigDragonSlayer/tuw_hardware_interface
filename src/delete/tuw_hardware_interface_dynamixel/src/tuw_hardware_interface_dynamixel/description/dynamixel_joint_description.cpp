// Copyright 2022 Eugen Kaltenegger

#include <tuw_hardware_interface_dynamixel/description/dynamixel_connection_description.h>
#include <tuw_hardware_interface_dynamixel/description/dynamixel_joint_description.h>

#include <tuw_hardware_interface_template/description/generic_hardware_description.h>
#include <tuw_hardware_interface_template/description/generic_config_description.h>
#include <tuw_hardware_interface_template/generic_setup_prefix.h>

#include <memory>
#include <string>

using tuw_hardware_interface::DynamixelConnectionDescription;
using tuw_hardware_interface::DynamixelJointDescription;

using tuw_hardware_interface::GenericHardwareDescription;
using tuw_hardware_interface::GenericConfigDescription;

DynamixelJointDescription::DynamixelJointDescription(const YAML::Node& yaml)
{
  this->id_ = yaml["id"].as<int>();
  this->name_ = yaml["name"].as<std::string>();

  if (yaml["connection"].IsDefined())
    this->dynamixel_connection_description_ =
            std::make_shared<DynamixelConnectionDescription>(yaml["connection"]);
  else
    ROS_ERROR("[%s] missing connection description", PREFIX);

  if (yaml["hardware"].IsDefined())
    this->hardware_description_ =
            std::make_shared<GenericHardwareDescription>(loadFile(yaml["hardware"]));
  else
    ROS_ERROR("[%s] missing hardware description", PREFIX);

  if (yaml["config"].IsDefined())
    this->config_description_ =
            std::make_shared<GenericConfigDescription>(loadFile(yaml["config"]));
  else
    ROS_INFO("[%s] no config provided - fallback to present hardware config", PREFIX);
}

std::shared_ptr<DynamixelConnectionDescription> DynamixelJointDescription::getDynamixelConnectionDescription()
{
  return this->dynamixel_connection_description_;
}
