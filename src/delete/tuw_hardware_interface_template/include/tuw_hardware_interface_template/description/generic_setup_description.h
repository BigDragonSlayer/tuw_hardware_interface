// Copyright 2022 Eugen Kaltenegger

#ifndef TUW_HARDWARE_INTERFACE_TEMPLATE_DESCRIPTION_GENERIC_SETUP_DESCRIPTION_H
#define TUW_HARDWARE_INTERFACE_TEMPLATE_DESCRIPTION_GENERIC_SETUP_DESCRIPTION_H

#include <list>
#include <string>

#include <yaml-cpp/yaml.h>

namespace tuw_hardware_interface
{
class GenericJointDescription;
class GenericSetupDescription
{
public:
  GenericSetupDescription() = default;
  ~GenericSetupDescription() = default;
  explicit GenericSetupDescription(YAML::Node yaml);
  std::string getName();
  std::list<GenericJointDescription> getJoints();
protected:
  std::string name_;
  std::list<GenericJointDescription> joints_ {std::list<GenericJointDescription>()};
};
}  // namespace tuw_hardware_interface


#endif  // TUW_HARDWARE_INTERFACE_TEMPLATE_DESCRIPTION_GENERIC_SETUP_DESCRIPTION_H
