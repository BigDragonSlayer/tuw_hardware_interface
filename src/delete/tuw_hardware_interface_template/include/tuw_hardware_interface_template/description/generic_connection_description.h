// Copyright 2022 Eugen Kaltenegger

#ifndef TUW_HARDWARE_INTERFACE_TEMPLATE_DESCRIPTION_GENERIC_CONNECTION_DESCRIPTION_H
#define TUW_HARDWARE_INTERFACE_TEMPLATE_DESCRIPTION_GENERIC_CONNECTION_DESCRIPTION_H

#include <string>

#include <yaml-cpp/yaml.h>

namespace tuw_hardware_interface
{
class GenericConnectionDescription
{
public:
  explicit GenericConnectionDescription(YAML::Node yaml);
  std::string getHash();
  std::string getPort();
  int getBaudrate() const;
private:
  std::string hash_;
  std::string port_;
  int baudrate_;
};
}  // namespace tuw_hardware_interface

#endif  // TUW_HARDWARE_INTERFACE_TEMPLATE_DESCRIPTION_GENERIC_CONNECTION_DESCRIPTION_H
