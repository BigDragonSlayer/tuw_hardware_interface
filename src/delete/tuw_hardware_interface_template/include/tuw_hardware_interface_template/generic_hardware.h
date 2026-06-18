// Copyright 2022 Eugen Kaltenegger

#ifndef TUW_HARDWARE_INTERFACE_TEMPLATE_GENERIC_HARDWARE_H
#define TUW_HARDWARE_INTERFACE_TEMPLATE_GENERIC_HARDWARE_H

#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <string>

#include <ros/ros.h>

#include <tuw_hardware_interface_template/description/generic_hardware_description.h>
#include <tuw_hardware_interface_template/generic_hardware_parameter.h>

namespace tuw_hardware_interface
{
class GenericHardware
{
public:
  // singleton functions
  static std::shared_ptr<GenericHardware> getHardware(GenericHardwareDescription hardware_description);
  // instance variables
  explicit GenericHardware(GenericHardwareDescription hardware_description);
  GenericHardware() = default;
  ~GenericHardware() = default;
  enum Mode
  {
    POSITION,
    VELOCITY,
    EFFORT
  };
  virtual std::string getName();
  virtual bool supportsTargetMode(Mode mode);
  virtual bool supportsActualMode(Mode mode);
  virtual std::vector<Mode> getSupportedTargetModes();
  virtual std::vector<Mode> getSupportedActualModes();
  virtual GenericHardwareParameter getTargetParameterForMode(Mode mode);
  virtual GenericHardwareParameter getActualParameterForMode(Mode mode);
  virtual std::shared_ptr<std::list<std::string>> getConfigIdentifiers();
  virtual std::shared_ptr<std::map<std::string, GenericHardwareParameter>> getConfigIdentifierToParameter();
  virtual int convertToHardwareResolution(double input, Mode mode);
  virtual double convertFromHardwareResolution(int input, Mode mode);
  static std::string modeToString(Mode mode);
  static Mode modeFromString(std::string mode_string);
protected:
  // singleton variables
  static std::mutex mutex_;
  static std::unique_ptr<std::map<std::string, std::shared_ptr<GenericHardware>>> hardware_table_;
  // instance variables
  std::string name_;
  std::map<Mode, double> modes_to_resolution_;
  std::map<Mode, GenericHardwareParameter> target_modes_to_parameter_;
  std::map<Mode, GenericHardwareParameter> actual_modes_to_parameter_;
  std::shared_ptr<std::list<std::string>> config_identifiers_;
  std::shared_ptr<std::map<std::string, GenericHardwareParameter>> config_identifier_to_parameter_;
};
}  // namespace tuw_hardware_interface

#endif  // TUW_HARDWARE_INTERFACE_TEMPLATE_GENERIC_HARDWARE_H
