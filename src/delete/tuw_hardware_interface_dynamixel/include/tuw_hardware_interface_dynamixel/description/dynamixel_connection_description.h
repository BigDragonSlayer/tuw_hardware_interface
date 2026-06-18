// Copyright 2022 Eugen Kaltenegger

#ifndef TUW_HARDWARE_INTERFACE_DYNAMIXEL_DESCRIPTION_DYNAMIXEL_CONNECTION_DESCRIPTION_H
#define TUW_HARDWARE_INTERFACE_DYNAMIXEL_DESCRIPTION_DYNAMIXEL_CONNECTION_DESCRIPTION_H

#include <tuw_hardware_interface_template/description/generic_connection_description.h>

#include <string>

namespace tuw_hardware_interface
{
class DynamixelConnectionDescription : public GenericConnectionDescription
{
public:
  explicit DynamixelConnectionDescription(YAML::Node yaml);
  std::string getProtocol() const;
private:
  std::string protocol_;
};
}  // namespace tuw_hardware_interface

#endif  // TUW_HARDWARE_INTERFACE_DYNAMIXEL_DESCRIPTION_DYNAMIXEL_CONNECTION_DESCRIPTION_H
