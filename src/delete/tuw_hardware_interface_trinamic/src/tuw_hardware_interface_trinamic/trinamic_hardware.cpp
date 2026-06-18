// Copyright 2022 Eugen Kaltenegger

#include "tuw_hardware_interface_trinamic/trinamic_hardware.h"
#include "tuw_hardware_interface_template/generic_setup_prefix.h"

using tuw_hardware_interface::GenericHardware;
using tuw_hardware_interface::TrinamicHardware;
using tuw_hardware_interface::TrinamicHardwareParameter;

std::unique_ptr<std::map<std::string, std::shared_ptr<TrinamicHardware>>> TrinamicHardware::trinamic_hardware_table_;

std::shared_ptr<TrinamicHardware> TrinamicHardware::getHardware(TrinamicHardwareDescription hardware_description)
{
  std::string hardware_name = hardware_description.getName();

  if (TrinamicHardware::trinamic_hardware_table_ == nullptr)
  {
    TrinamicHardware::mutex_.lock();

    if (TrinamicHardware::trinamic_hardware_table_ == nullptr)

      TrinamicHardware::trinamic_hardware_table_ = std::make_unique<std::map<std::string, std::shared_ptr<TrinamicHardware>>>();

    TrinamicHardware::mutex_.unlock();
  }

  if (TrinamicHardware::trinamic_hardware_table_->find(hardware_name) == TrinamicHardware::trinamic_hardware_table_->end())
  {
    std::shared_ptr<TrinamicHardware> hardware = std::make_shared<TrinamicHardware>(hardware_description);
    TrinamicHardware::trinamic_hardware_table_->insert({hardware_name, hardware});
  }

  return TrinamicHardware::trinamic_hardware_table_->at(hardware_description.getName());
}

TrinamicHardware::TrinamicHardware(TrinamicHardwareDescription hardware_description)
{
  this->name_ = hardware_description.getName();
  this->modes_to_resolution_ = std::map<Mode, double>();
  this->target_modes_to_trinamic_parameter_ = std::map<Mode, TrinamicHardwareParameter>();
  this->actual_modes_to_trinamic_parameter_ = std::map<Mode, TrinamicHardwareParameter>();
  this->config_identifiers_ = std::make_shared<std::list<std::string>>();
  this->config_identifier_to_trinamic_parameter_ = std::make_shared<std::map<std::string, TrinamicHardwareParameter>>();

  if (hardware_description.getPositionResolution())
    this->modes_to_resolution_.insert({Mode::POSITION, *hardware_description.getPositionResolution()});
  if (hardware_description.getVelocityResolution())
    this->modes_to_resolution_.insert({Mode::VELOCITY, *hardware_description.getVelocityResolution()});
  if (hardware_description.getEffortResolution())
    this->modes_to_resolution_.insert({Mode::EFFORT, *hardware_description.getEffortResolution()});

  for (const auto& key_value_pair : *hardware_description.getTargetIdentifierToTrinamicDescription())
  {
    auto key = key_value_pair.first;
    auto value = key_value_pair.second;
    if (key == "target_position")
    {
      if (this->modes_to_resolution_.find(Mode::POSITION) != this->modes_to_resolution_.end())
        this->target_modes_to_trinamic_parameter_.insert({Mode::POSITION, TrinamicHardwareParameter(value)});
      else
        ROS_WARN("[%s] target mode POSITION requires position resolution", PREFIX);
    }
    else if (key == "target_velocity")
    {
      if (this->modes_to_resolution_.find(Mode::VELOCITY) != this->modes_to_resolution_.end())
        this->target_modes_to_trinamic_parameter_.insert({Mode::VELOCITY, TrinamicHardwareParameter(value)});
      else
        ROS_WARN("[%s] target mode VELOCITY requires position resolution", PREFIX);
    }
    else if (key == "target_effort")
    {
      if (this->modes_to_resolution_.find(Mode::EFFORT) != this->modes_to_resolution_.end())
        this->target_modes_to_trinamic_parameter_.insert({Mode::EFFORT, TrinamicHardwareParameter(value)});
      else
        ROS_WARN("[%s] target mode EFFORT requires position resolution", PREFIX);
    }
  }

  for (const auto& key_value_pair : *hardware_description.getActualIdentifierToTrinamicDescription())
  {
    auto key = key_value_pair.first;
    auto value = key_value_pair.second;
    if (key == "actual_position")
    {
      if (this->modes_to_resolution_.find(Mode::POSITION) != this->modes_to_resolution_.end())
        this->actual_modes_to_trinamic_parameter_.insert({Mode::POSITION, TrinamicHardwareParameter(value)});
      else
        ROS_WARN("[%s] actual mode POSITION requires position resolution", PREFIX);
    }
    else if (key == "actual_velocity")
    {
      if (this->modes_to_resolution_.find(Mode::VELOCITY) != this->modes_to_resolution_.end())
        this->actual_modes_to_trinamic_parameter_.insert({Mode::VELOCITY, TrinamicHardwareParameter(value)});
      else
        ROS_WARN("[%s] actual mode VELOCITY requires position resolution", PREFIX);
    }
    else if (key == "actual_effort")
    {
      if (this->modes_to_resolution_.find(Mode::EFFORT) != this->modes_to_resolution_.end())
        this->actual_modes_to_trinamic_parameter_.insert({Mode::EFFORT, TrinamicHardwareParameter(value)});
      else
        ROS_WARN("[%s] actual mode EFFORT requires position resolution", PREFIX);
    }
  }

  for (const auto& key : *hardware_description.getConfigIdentifiers())
  {
    this->config_identifiers_->emplace_back(key);
  }

  for (const auto& key_value_pair : *hardware_description.getConfigIdentifierToTrinamicDescription())
  {
    auto key = key_value_pair.first;
    auto value = key_value_pair.second;
    std::pair<std::string, TrinamicHardwareParameter> pair = {key, TrinamicHardwareParameter(value)};
    this->config_identifier_to_trinamic_parameter_->insert(pair);
  }
}

TrinamicHardwareParameter TrinamicHardware::getTargetTrinamicParameterForMode(GenericHardware::Mode mode)
{
  return this->target_modes_to_trinamic_parameter_.at(mode);
}

TrinamicHardwareParameter TrinamicHardware::getActualTrinamicParameterForMode(GenericHardware::Mode mode)
{
  return this->actual_modes_to_trinamic_parameter_.at(mode);
}

std::shared_ptr<std::map<std::string, TrinamicHardwareParameter>>
TrinamicHardware::getConfigIdentifierToTrinamicParameter()
{
  return this->config_identifier_to_trinamic_parameter_;
}

bool tuw_hardware_interface::TrinamicHardware::supportsTargetMode(GenericHardware::Mode mode)
{
  if (this->target_modes_to_trinamic_parameter_.find(mode) == this->target_modes_to_trinamic_parameter_.end())
    return false;
  else
    return true;
}

bool tuw_hardware_interface::TrinamicHardware::supportsActualMode(GenericHardware::Mode mode)
{
  if (this->actual_modes_to_trinamic_parameter_.find(mode) == this->actual_modes_to_trinamic_parameter_.end())
    return false;
  else
    return true;
}

std::vector<GenericHardware::Mode> tuw_hardware_interface::TrinamicHardware::getSupportedTargetModes()
{
  std::vector<GenericHardware::Mode> supported_modes;
  for (const auto& modes_to_parameter : this->target_modes_to_trinamic_parameter_)
  {
    supported_modes.push_back(modes_to_parameter.first);
  }
  return supported_modes;
}

std::vector<GenericHardware::Mode> tuw_hardware_interface::TrinamicHardware::getSupportedActualModes()
{
  std::vector<GenericHardware::Mode> supported_modes;
  for (const auto& modes_to_parameter : this->actual_modes_to_trinamic_parameter_)
  {
    supported_modes.push_back(modes_to_parameter.first);
  }
  return supported_modes;
}
