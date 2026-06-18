// Copyright 2022 Eugen Kaltenegger

#ifndef TUW_HARDWARE_INTERFACE_TEMPLATE_GENERIC_SETUP_PREFIX_H
#define TUW_HARDWARE_INTERFACE_TEMPLATE_GENERIC_SETUP_PREFIX_H

#include <string>
#include <memory>

#include <ros/ros.h>

#define LOG c_str()
#define PREFIX tuw_hardware_interface::GenericSetupPrefix::getLogPrefix().LOG

namespace tuw_hardware_interface
{
class GenericSetupPrefix
{
public:
  GenericSetupPrefix() = default;
  ~GenericSetupPrefix() = default;
  static void setSetupName(const std::string& setup_name);
  static std::string getSetupName();
  static std::string getNodeName();
  static std::string getNodeNameLower();
  static std::string getNodeNameUpper();
  static std::string getLogPrefix();
protected:
  static std::string node_name_;
  static std::string setup_name_;
};
}  // namespace tuw_hardware_interface

#endif  // TUW_HARDWARE_INTERFACE_TEMPLATE_GENERIC_SETUP_PREFIX_H
