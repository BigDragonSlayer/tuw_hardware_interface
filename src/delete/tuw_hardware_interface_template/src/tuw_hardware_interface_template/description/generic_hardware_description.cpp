// Copyright 2022 Eugen Kaltenegger

#include <tuw_hardware_interface_template/description/generic_hardware_description.h>
#include <tuw_hardware_interface_template/description/generic_hardware_parameter_description.h>

#include <list>
#include <map>
#include <memory>
#include <string>

using tuw_hardware_interface::GenericHardwareDescription;
using tuw_hardware_interface::GenericHardwareParameterDescription;

GenericHardwareDescription::GenericHardwareDescription(YAML::Node yaml)
{
  this->name_ = yaml["name"].as<std::string>();

  if (yaml["resolution"]["position"].IsDefined())
    this->position_resolution_ = std::make_shared<double>(yaml["resolution"]["position"].as<double>());

  if (yaml["resolution"]["velocity"].IsDefined())
    this->velocity_resolution_ = std::make_shared<double>(yaml["resolution"]["velocity"].as<double>());

  if (yaml["resolution"]["effort"].IsDefined())
    this->effort_resolution_ = std::make_shared<double>(yaml["resolution"]["effort"].as<double>());

  this->target_identifier_to_description_ =
          std::make_shared<std::map<std::string, GenericHardwareParameterDescription>>();
  this->actual_identifier_to_description_ =
          std::make_shared<std::map<std::string, GenericHardwareParameterDescription>>();
  this->config_identifier_to_description_ =
          std::make_shared<std::map<std::string, GenericHardwareParameterDescription>>();

  this->config_identifiers_ = std::make_shared<std::list<std::string>>();

  YAML::Node target_values_yaml = yaml["target_state_parameters"];
  YAML::Node actual_values_yaml = yaml["actual_state_parameters"];
  YAML::Node config_values_yaml = yaml["config_parameters"];

  for (auto target_value_yaml : target_values_yaml)
  {
    std::string key = target_value_yaml["identifier"].as<std::string>();
    this->target_identifier_to_description_->insert({key, GenericHardwareParameterDescription(target_value_yaml)});
  }

  for (auto actual_value_yaml : actual_values_yaml)
  {
    std::string key = actual_value_yaml["identifier"].as<std::string>();
    this->actual_identifier_to_description_->insert({key, GenericHardwareParameterDescription(actual_value_yaml)});
  }

  for (auto config_value_yaml : config_values_yaml)
  {
    std::string key = config_value_yaml["identifier"].as<std::string>();
    this->config_identifiers_->emplace_back(key);
    this->config_identifier_to_description_->insert({key, GenericHardwareParameterDescription(config_value_yaml)});
  }
}

std::string GenericHardwareDescription::getName()
{
  return this->name_;
}

std::shared_ptr<double> GenericHardwareDescription::getPositionResolution()
{
  return this->position_resolution_;
}

std::shared_ptr<double> GenericHardwareDescription::getVelocityResolution()
{
  return this->velocity_resolution_;
}

std::shared_ptr<double> GenericHardwareDescription::getEffortResolution()
{
  return this->effort_resolution_;
}

std::shared_ptr<std::map<std::string, GenericHardwareParameterDescription>>
GenericHardwareDescription::getTargetIdentifierToDescription()
{
  return this->target_identifier_to_description_;
}

std::shared_ptr<std::map<std::string, GenericHardwareParameterDescription>>
GenericHardwareDescription::getActualIdentifierToDescription()
{
  return this->actual_identifier_to_description_;
}

std::shared_ptr<std::map<std::string, GenericHardwareParameterDescription>>
GenericHardwareDescription::getConfigIdentifierToDescription()
{
  return this->config_identifier_to_description_;
}

std::shared_ptr<std::list<std::string>> GenericHardwareDescription::getConfigIdentifiers()
{
  return this->config_identifiers_;
}
