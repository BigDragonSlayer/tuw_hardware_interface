// Copyright 2022 Eugen Kaltenegger

#include <tuw_hardware_interface_trinamic/description/trinamic_setup_description.h>

using tuw_hardware_interface::TrinamicJointDescription;
using tuw_hardware_interface::TrinamicSetupDescription;

TrinamicSetupDescription::TrinamicSetupDescription(YAML::Node yaml)
{
  this->name_ = yaml["setup"].as<std::string>();
  for (const auto& joint_yaml : yaml["joints"])
  {
    TrinamicJointDescription joint_description = TrinamicJointDescription(joint_yaml);
    this->trinamic_joints_.emplace_back(joint_description);
  }
}

std::list<TrinamicJointDescription> TrinamicSetupDescription::getTrinamicJoints()
{
  return trinamic_joints_;
}
