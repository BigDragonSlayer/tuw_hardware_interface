// Copyright 2022 Eugen Kaltenegger

#ifndef TUW_HARDWARE_INTERFACE_TEMPLATE_DESCRIPTION_GENERIC_JOINT_DESCRIPTION_H
#define TUW_HARDWARE_INTERFACE_TEMPLATE_DESCRIPTION_GENERIC_JOINT_DESCRIPTION_H

#include <memory>
#include <string>

#include <yaml-cpp/yaml.h>

#include <tuw_hardware_interface_template/description/generic_connection_description.h>
#include <tuw_hardware_interface_template/description/generic_hardware_description.h>
#include <tuw_hardware_interface_template/description/generic_config_description.h>

namespace tuw_hardware_interface
{
class GenericJointDescription
{
public:
  GenericJointDescription() = default;
  ~GenericJointDescription() = default;
  explicit GenericJointDescription(YAML::Node yaml);
  virtual std::string getName();
  virtual int getId();
  virtual double getDiameter();
  virtual std::shared_ptr<GenericConnectionDescription> getConnectionDescription();
  virtual std::shared_ptr<GenericHardwareDescription> getHardwareDescription();
  virtual std::shared_ptr<GenericConfigDescription> getConfigDescription();
protected:
  static YAML::Node loadFile(YAML::Node yaml);
  std::string name_;
  int id_ {0};
  double diameter_ {0};
  std::shared_ptr<GenericConnectionDescription> connection_description_ {nullptr};
  std::shared_ptr<GenericHardwareDescription> hardware_description_ {nullptr};
  std::shared_ptr<GenericConfigDescription> config_description_ {nullptr};
};
}  // namespace tuw_hardware_interface

#endif  // TUW_HARDWARE_INTERFACE_TEMPLATE_DESCRIPTION_GENERIC_JOINT_DESCRIPTION_H
