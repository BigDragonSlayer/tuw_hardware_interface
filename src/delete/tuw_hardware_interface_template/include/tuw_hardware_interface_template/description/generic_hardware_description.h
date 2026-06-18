// Copyright 2022 Eugen Kaltenegger

#ifndef TUW_HARDWARE_INTERFACE_TEMPLATE_DESCRIPTION_GENERIC_HARDWARE_DESCRIPTION_H
#define TUW_HARDWARE_INTERFACE_TEMPLATE_DESCRIPTION_GENERIC_HARDWARE_DESCRIPTION_H

#include <list>
#include <map>
#include <memory>
#include <string>

#include <boost/unordered_map.hpp>

#include <yaml-cpp/yaml.h>

namespace tuw_hardware_interface
{
class GenericHardwareParameterDescription;

class GenericHardwareDescription
{
public:
  GenericHardwareDescription() = default;
  ~GenericHardwareDescription() = default;
  explicit GenericHardwareDescription(YAML::Node yaml);
  virtual std::string getName();
  virtual std::shared_ptr<double> getPositionResolution();
  virtual std::shared_ptr<double> getVelocityResolution();
  virtual std::shared_ptr<double> getEffortResolution();
  virtual std::shared_ptr<std::map<std::string, GenericHardwareParameterDescription>> getTargetIdentifierToDescription();
  virtual std::shared_ptr<std::map<std::string, GenericHardwareParameterDescription>> getActualIdentifierToDescription();
  virtual std::shared_ptr<std::map<std::string, GenericHardwareParameterDescription>> getConfigIdentifierToDescription();
  virtual std::shared_ptr<std::list<std::string>> getConfigIdentifiers();
protected:
  std::string name_;
  std::shared_ptr<double> position_resolution_;
  std::shared_ptr<double> velocity_resolution_;
  std::shared_ptr<double> effort_resolution_;
  std::shared_ptr<std::map<std::string, GenericHardwareParameterDescription>>
          target_identifier_to_description_{nullptr};
  std::shared_ptr<std::map<std::string, GenericHardwareParameterDescription>>
          actual_identifier_to_description_{nullptr};
  std::shared_ptr<std::map<std::string, GenericHardwareParameterDescription>>
          config_identifier_to_description_{nullptr};
  std::shared_ptr<std::list<std::string>> config_identifiers_;
};
}  // namespace tuw_hardware_interface

#endif  // TUW_HARDWARE_INTERFACE_TEMPLATE_DESCRIPTION_GENERIC_HARDWARE_DESCRIPTION_H
