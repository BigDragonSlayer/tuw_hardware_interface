// Copyright 2022 Eugen Kaltenegger

#ifndef TUW_HARDWARE_INTERFACE_TRINAMIC_DESCRIPTION_TRINAMIC_HARDWARE_PARAMETER_DESCRIPTION_H
#define TUW_HARDWARE_INTERFACE_TRINAMIC_DESCRIPTION_TRINAMIC_HARDWARE_PARAMETER_DESCRIPTION_H

#include <map>
#include <memory>
#include <string>

#include <yaml-cpp/yaml.h>
#include "tuw_hardware_interface_template/generic_hardware_parameter.h"

namespace tuw_hardware_interface
{
class TrinamicHardwareParameterDescription : public GenericHardwareParameterDescription
{
public:
  explicit TrinamicHardwareParameterDescription(YAML::Node yaml);
  std::shared_ptr<int> getParameter();
protected:
  std::shared_ptr<int> parameter_ {nullptr};
};
}  // namespace tuw_hardware_interface

#endif  // TUW_HARDWARE_INTERFACE_TRINAMIC_DESCRIPTION_TRINAMIC_HARDWARE_PARAMETER_DESCRIPTION_H
