// Copyright 2022 Eugen Kaltenegger

#ifndef TUW_HARDWARE_INTERFACE_TEMPLATE_DESCRIPTION_GENERIC_CONFIG_DESCRIPTION_H
#define TUW_HARDWARE_INTERFACE_TEMPLATE_DESCRIPTION_GENERIC_CONFIG_DESCRIPTION_H

#include <map>
#include <string>

#include <yaml-cpp/yaml.h>

namespace tuw_hardware_interface
{
class GenericConfigDescription
{
public:
  explicit GenericConfigDescription(const YAML::Node& yaml);
  std::vector<std::pair<std::string, int>> getConfig();
private:
  std::vector<std::pair<std::string, int>> config_;
};
}  // namespace tuw_hardware_interface

#endif  // TUW_HARDWARE_INTERFACE_TEMPLATE_DESCRIPTION_GENERIC_CONFIG_DESCRIPTION_H
