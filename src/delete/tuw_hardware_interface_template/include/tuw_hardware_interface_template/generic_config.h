// Copyright 2022 Eugen Kaltenegger

#ifndef TUW_HARDWARE_INTERFACE_TEMPLATE_GENERIC_CONFIG_H
#define TUW_HARDWARE_INTERFACE_TEMPLATE_GENERIC_CONFIG_H

#include <map>
#include <memory>
#include <string>

#include <ddynamic_reconfigure/ddynamic_reconfigure.h>

#include <tuw_hardware_interface_template/description/generic_config_description.h>
#include <tuw_hardware_interface_template/description/generic_hardware_parameter_description.h>

namespace tuw_hardware_interface
{
class GenericJoint;
class GenericHardware;
class GenericHardwareParameter;
class GenericConfig
{
public:
  GenericConfig() = default;
  ~GenericConfig() = default;
  GenericConfig(std::shared_ptr<GenericJoint> joint,
                std::shared_ptr<GenericHardware> hardware);
  GenericConfig(std::shared_ptr<GenericJoint> joint,
                std::shared_ptr<GenericHardware> hardware,
                GenericConfigDescription config_description);
protected:
  virtual void setupReconfigureServer();
  virtual void setInitialConfig(GenericConfigDescription config_description);
  virtual void reconfigureConfig();
  virtual void reconfigureValue(GenericHardwareParameter parameter, int target_value);

  void registerReconfigureVariable(GenericHardwareParameter hardware_parameter);
  void registerReconfigureEnumVariable(GenericHardwareParameter hardware_parameter);
  void registerReconfigureRangeVariable(GenericHardwareParameter hardware_parameter);

  std::shared_ptr<GenericJoint> joint_ {nullptr};
  std::shared_ptr<GenericHardware> hardware_ {nullptr};

  std::unique_ptr<ddynamic_reconfigure::DDynamicReconfigure> reconfigure_ {nullptr};

  std::map<std::string, int> target_config_values_ {std::map<std::string, int>()};
  std::map<std::string, int> actual_config_values_ {std::map<std::string, int>()};
};
}  // namespace tuw_hardware_interface

#endif  // TUW_HARDWARE_INTERFACE_TEMPLATE_GENERIC_CONFIG_H
