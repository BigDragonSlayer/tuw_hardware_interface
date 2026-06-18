// Copyright 2022 Eugen Kaltenegger

#ifndef TUW_HARDWARE_INTERFACE_TEMPLATE_UTILS_FILE_LOADER_H
#define TUW_HARDWARE_INTERFACE_TEMPLATE_UTILS_FILE_LOADER_H

#include <fstream>
#include <string>

#include <ros/package.h>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>

namespace tuw_hardware_interface
{
class FileLoader
{
public:
  static YAML::Node loadYAMLFromFile(const std::string& relative_path);
  static std::string loadURDFFromFile(const std::string& relative_path);
private:
};
}  // namespace tuw_hardware_interface

#endif  // TUW_HARDWARE_INTERFACE_TEMPLATE_UTILS_FILE_LOADER_H
