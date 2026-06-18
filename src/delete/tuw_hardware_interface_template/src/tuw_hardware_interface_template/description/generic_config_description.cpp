// Copyright 2022 Eugen Kaltenegger

#include <tuw_hardware_interface_template/description/generic_config_description.h>

#include <map>
#include <string>
#include <ros/ros.h>

using tuw_hardware_interface::GenericConfigDescription;

GenericConfigDescription::GenericConfigDescription(const YAML::Node& yaml)
{
  YAML::Node config_yaml = yaml["config"];
  if (config_yaml.IsDefined() && !config_yaml.IsNull())
  {
    for (YAML::Node config_info : config_yaml)
    {
      std::pair<std::string, int> pair = {config_info["identifier"].as<std::string>(), config_info["value"].as<int>()};
      this->config_.emplace_back(pair);
    }
  }
  else
  {
    ROS_WARN("config is invalid!");
  }
}

std::vector<std::pair<std::string, int>> GenericConfigDescription::getConfig()
{
  return this->config_;
}
