// Copyright 2022 Eugen Kaltenegger

#include <tuw_hardware_interface_trinamic/description/trinamic_joint_description.h>
#include <tuw_hardware_interface_template/generic_setup_prefix.h>

using tuw_hardware_interface::TrinamicHardwareDescription;
using tuw_hardware_interface::TrinamicJointDescription;

TrinamicJointDescription::TrinamicJointDescription(const YAML::Node& yaml)
{
  this->id_ = yaml["id"].as<int>();
  this->name_ = yaml["name"].as<std::string>();

  if (yaml["diameter"].IsDefined() && !yaml["diameter"].IsNull())
  {
    this->diameter_ = yaml["diameter"].as<double>();
    ROS_INFO_NAMED(PREFIX, "joint %s has a diameter and will except meters per second as velocity input", this->name_.LOG);
  }
  else
  {
    ROS_INFO_NAMED(PREFIX, "joint %s has a diameter and will except meters per second as velocity input", this->name_.LOG);
  }

  if (yaml["connection"].IsDefined())
    this->connection_description_ =
            std::make_shared<GenericConnectionDescription>(yaml["connection"]);
  else
    ROS_ERROR("[%s] missing connection description", PREFIX);

  if (yaml["hardware"].IsDefined())
  {
    YAML::Node hardware_yaml = loadFile(yaml["hardware"]);
    this->trinamic_hardware_description_ = std::make_shared<TrinamicHardwareDescription>(hardware_yaml);
  }
  else
    ROS_ERROR("[%s] missing hardware description", PREFIX);

  if (yaml["config"].IsDefined())
    this->config_description_ =
            std::make_shared<GenericConfigDescription>(loadFile(yaml["config"]));
  else
    ROS_INFO("[%s] no config provided - fallback to present hardware config", PREFIX);
}

std::shared_ptr<TrinamicHardwareDescription>
TrinamicJointDescription::getTrinamicHardwareDescription()
{
  return this->trinamic_hardware_description_;
}


