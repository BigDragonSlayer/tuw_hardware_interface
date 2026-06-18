// Copyright 2022 Eugen Kaltenegger

#include <tuw_hardware_interface_template/generic_hardware.h>

#include <algorithm>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>

#include <tuw_hardware_interface_template/generic_setup_prefix.h>

using tuw_hardware_interface::GenericHardware;
using tuw_hardware_interface::GenericHardwareParameter;

std::mutex GenericHardware::mutex_;
std::unique_ptr<std::map<std::string, std::shared_ptr<GenericHardware>>> GenericHardware::hardware_table_;

std::shared_ptr<GenericHardware> GenericHardware::getHardware(GenericHardwareDescription hardware_description)
{
  std::string hardware_name = hardware_description.getName();

  if (GenericHardware::hardware_table_ == nullptr)
  {
    GenericHardware::mutex_.lock();

    if (GenericHardware::hardware_table_ == nullptr)

      GenericHardware::hardware_table_ = std::make_unique<std::map<std::string, std::shared_ptr<GenericHardware>>>();

    GenericHardware::mutex_.unlock();
  }

  if (GenericHardware::hardware_table_->find(hardware_name) == GenericHardware::hardware_table_->end())
  {
    std::shared_ptr<GenericHardware> hardware = std::make_shared<GenericHardware>(hardware_description);
    GenericHardware::hardware_table_->insert({hardware_name, hardware});
  }

  return GenericHardware::hardware_table_->at(hardware_description.getName());
}

GenericHardware::GenericHardware(GenericHardwareDescription hardware_description)
{
  this->name_ = hardware_description.getName();
  this->modes_to_resolution_ = std::map<Mode, double>();
  this->target_modes_to_parameter_ = std::map<Mode, GenericHardwareParameter>();
  this->actual_modes_to_parameter_ = std::map<Mode, GenericHardwareParameter>();
  this->config_identifiers_ = std::make_shared<std::list<std::string>>();
  this->config_identifier_to_parameter_ = std::make_shared<std::map<std::string, GenericHardwareParameter>>();

  if (hardware_description.getPositionResolution())
    this->modes_to_resolution_.insert({Mode::POSITION, *hardware_description.getPositionResolution()});
  if (hardware_description.getVelocityResolution())
    this->modes_to_resolution_.insert({Mode::VELOCITY, *hardware_description.getVelocityResolution()});
  if (hardware_description.getEffortResolution())
    this->modes_to_resolution_.insert({Mode::EFFORT, *hardware_description.getEffortResolution()});

  for (const auto& key_value_pair : *hardware_description.getTargetIdentifierToDescription())
  {
    auto key = key_value_pair.first;
    auto value = key_value_pair.second;
    if (key == "target_position")
    {
      if (this->modes_to_resolution_.find(Mode::POSITION) != this->modes_to_resolution_.end())
        this->target_modes_to_parameter_.insert({Mode::POSITION, GenericHardwareParameter(value)});
      else
        ROS_WARN("[%s] target mode POSITION requires position resolution", PREFIX);
    }
    else if (key == "target_velocity")
    {
      if (this->modes_to_resolution_.find(Mode::VELOCITY) != this->modes_to_resolution_.end())
        this->target_modes_to_parameter_.insert({Mode::VELOCITY, GenericHardwareParameter(value)});
      else
        ROS_WARN("[%s] target mode VELOCITY requires position resolution", PREFIX);
    }
    else if (key == "target_effort")
    {
      if (this->modes_to_resolution_.find(Mode::EFFORT) != this->modes_to_resolution_.end())
        this->target_modes_to_parameter_.insert({Mode::EFFORT, GenericHardwareParameter(value)});
      else
        ROS_WARN("[%s] target mode EFFORT requires position resolution", PREFIX);
    }
  }

  for (const auto& key_value_pair : *hardware_description.getActualIdentifierToDescription())
  {
    auto key = key_value_pair.first;
    auto value = key_value_pair.second;
    if (key == "actual_position")
    {
      if (this->modes_to_resolution_.find(Mode::POSITION) != this->modes_to_resolution_.end())
        this->actual_modes_to_parameter_.insert({Mode::POSITION, GenericHardwareParameter(value)});
      else
        ROS_WARN("[%s] actual mode POSITION requires position resolution", PREFIX);
    }
    else if (key == "actual_velocity")
    {
      if (this->modes_to_resolution_.find(Mode::VELOCITY) != this->modes_to_resolution_.end())
        this->actual_modes_to_parameter_.insert({Mode::VELOCITY, GenericHardwareParameter(value)});
      else
        ROS_WARN("[%s] actual mode VELOCITY requires position resolution", PREFIX);
    }
    else if (key == "actual_effort")
    {
      if (this->modes_to_resolution_.find(Mode::EFFORT) != this->modes_to_resolution_.end())
        this->actual_modes_to_parameter_.insert({Mode::EFFORT, GenericHardwareParameter(value)});
      else
        ROS_WARN("[%s] actual mode EFFORT requires position resolution", PREFIX);
    }
  }

  for (const auto& key : *hardware_description.getConfigIdentifiers())
  {
    this->config_identifiers_->emplace_back(key);
  }

  for (const auto& key_value_pair : *hardware_description.getConfigIdentifierToDescription())
  {
    auto key = key_value_pair.first;
    auto value = key_value_pair.second;
    this->config_identifier_to_parameter_->insert({key, GenericHardwareParameter(value)});
  }
}

std::string GenericHardware::getName()
{
  return this->name_;
}

bool GenericHardware::supportsTargetMode(GenericHardware::Mode mode)
{
  if (this->target_modes_to_parameter_.find(mode) == this->target_modes_to_parameter_.end())
    return false;
  else
    return true;
}

bool GenericHardware::supportsActualMode(GenericHardware::Mode mode)
{
  if (this->actual_modes_to_parameter_.find(mode) == this->actual_modes_to_parameter_.end())
    return false;
  else
    return true;
}

GenericHardwareParameter GenericHardware::getTargetParameterForMode(GenericHardware::Mode mode)
{
  return this->target_modes_to_parameter_.at(mode);
}

GenericHardwareParameter GenericHardware::getActualParameterForMode(GenericHardware::Mode mode)
{
  return this->actual_modes_to_parameter_.at(mode);
}

std::shared_ptr<std::list<std::string>> GenericHardware::getConfigIdentifiers()
{
  return this->config_identifiers_;
}

std::shared_ptr<std::map<std::string, GenericHardwareParameter>> GenericHardware::getConfigIdentifierToParameter()
{
  return this->config_identifier_to_parameter_;
}

int GenericHardware::convertToHardwareResolution(double input, GenericHardware::Mode mode)
{
  return static_cast<int>(input / this->modes_to_resolution_.at(mode));
}

double GenericHardware::convertFromHardwareResolution(int input, GenericHardware::Mode mode)
{
  return static_cast<double>(input) * this->modes_to_resolution_.at(mode);
}

std::string GenericHardware::modeToString(GenericHardware::Mode mode)
{
  switch (mode)
  {
    case GenericHardware::Mode::POSITION:
      return "POSITION";
    case GenericHardware::Mode::VELOCITY:
      return "VELOCITY";
    case GenericHardware::Mode::EFFORT:
      return "EFFORT";
    default:
      throw std::runtime_error("unknown mode");
  }
}

GenericHardware::Mode GenericHardware::modeFromString(std::string mode_string)
{
  std::string upper_mode_string = mode_string;
  auto to_upper_begin = upper_mode_string.begin();
  auto to_upper_end = upper_mode_string.end();
  std::transform(to_upper_begin, to_upper_end, upper_mode_string.begin(), std::ptr_fun<int, int>(std::toupper));

  if (upper_mode_string.find("POSITION") != std::string::npos)
    return GenericHardware::Mode::POSITION;
  if (upper_mode_string.find("VELOCITY") != std::string::npos)
    return GenericHardware::Mode::VELOCITY;
  if (upper_mode_string.find("EFFORT") != std::string::npos)
    return GenericHardware::Mode::EFFORT;
  throw std::runtime_error("unknown mode: " + mode_string);
}

std::vector<GenericHardware::Mode> GenericHardware::getSupportedTargetModes()
{
  std::vector<Mode> supported_modes;
  for (const auto& modes_to_parameter : this->target_modes_to_parameter_)
  {
    supported_modes.push_back(modes_to_parameter.first);
  }
  return supported_modes;
}

std::vector<GenericHardware::Mode> GenericHardware::getSupportedActualModes()
{
  std::vector<Mode> supported_modes;
  for (const auto& modes_to_parameter : this->actual_modes_to_parameter_)
  {
    supported_modes.push_back(modes_to_parameter.first);
  }
  return supported_modes;
}
