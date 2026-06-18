// Copyright 2022 Eugen Kaltenegger

#include "tuw_hardware_interface_trinamic/description/trinamic_hardware_description.h"

using tuw_hardware_interface::TrinamicHardwareDescription;
using tuw_hardware_interface::TrinamicHardwareParameterDescription;

tuw_hardware_interface::TrinamicHardwareDescription::TrinamicHardwareDescription(YAML::Node yaml)
{
  this->name_ = yaml["name"].as<std::string>();

  if (yaml["resolution"]["position"].IsDefined())
    this->position_resolution_ = std::make_shared<double>(yaml["resolution"]["position"].as<double>());

  if (yaml["resolution"]["velocity"].IsDefined())
    this->velocity_resolution_ = std::make_shared<double>(yaml["resolution"]["velocity"].as<double>());

  if (yaml["resolution"]["effort"].IsDefined())
    this->effort_resolution_ = std::make_shared<double>(yaml["resolution"]["effort"].as<double>());

  this->target_identifier_to_trinamic_description_ =
          std::make_shared<std::map<std::string, TrinamicHardwareParameterDescription>>();
  this->actual_identifier_to_trinamic_description_ =
          std::make_shared<std::map<std::string, TrinamicHardwareParameterDescription>>();
  this->config_identifier_to_trinamic_description_ =
          std::make_shared<std::map<std::string, TrinamicHardwareParameterDescription>>();

  this->config_identifiers_ = std::make_shared<std::list<std::string>>();

  YAML::Node target_values_yaml = yaml["target_state_parameters"];
  YAML::Node actual_values_yaml = yaml["actual_state_parameters"];
  YAML::Node config_values_yaml = yaml["config_parameters"];

  for (auto target_value_yaml : target_values_yaml)
  {
    std::string key = target_value_yaml["identifier"].as<std::string>();
    this->target_identifier_to_trinamic_description_->insert({key, TrinamicHardwareParameterDescription(target_value_yaml)});
  }

  for (auto actual_value_yaml : actual_values_yaml)
  {
    std::string key = actual_value_yaml["identifier"].as<std::string>();
    this->actual_identifier_to_trinamic_description_->insert({key, TrinamicHardwareParameterDescription(actual_value_yaml)});
  }

  for (auto config_value_yaml : config_values_yaml)
  {
    std::string key = config_value_yaml["identifier"].as<std::string>();
    this->config_identifiers_->emplace_back(key);
    this->config_identifier_to_trinamic_description_->insert({key, TrinamicHardwareParameterDescription(config_value_yaml)});
  }
}

std::shared_ptr<std::map<std::string, TrinamicHardwareParameterDescription>>
tuw_hardware_interface::TrinamicHardwareDescription::getTargetIdentifierToTrinamicDescription()
{
  return this->target_identifier_to_trinamic_description_;
}

std::shared_ptr<std::map<std::string, TrinamicHardwareParameterDescription>>
tuw_hardware_interface::TrinamicHardwareDescription::getActualIdentifierToTrinamicDescription()
{
  return this->actual_identifier_to_trinamic_description_;
}

std::shared_ptr<std::map<std::string, TrinamicHardwareParameterDescription>>
TrinamicHardwareDescription::getConfigIdentifierToTrinamicDescription()
{
  return this->config_identifier_to_trinamic_description_;
}
