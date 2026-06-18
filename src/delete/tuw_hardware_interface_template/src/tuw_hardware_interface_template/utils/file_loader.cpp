// Copyright 2022 Eugen Kaltenegger

#include "tuw_hardware_interface_template/utils/file_loader.h"

#include <string>

using tuw_hardware_interface::FileLoader;

YAML::Node FileLoader::loadYAMLFromFile(const std::string& relative_path)
{
  {
    std::string absolute_package_path = ros::package::getPath("tuw_hardware_interface_template");
    std::string absolute_file_path = absolute_package_path + relative_path;
    return YAML::LoadFile(absolute_file_path);
  }
}

std::string FileLoader::loadURDFFromFile(const std::string &relative_path)
{
  std::string absolute_package_path = ros::package::getPath("tuw_hardware_interface_template");
  std::string absolute_file_path = absolute_package_path + relative_path;

  std::ifstream input_file(absolute_file_path);
  if (input_file.is_open())
    return std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
  else
    throw std::runtime_error("can not open file: " + absolute_file_path);
}
