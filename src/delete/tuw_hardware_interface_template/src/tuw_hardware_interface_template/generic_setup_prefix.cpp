// Copyright 2022 Eugen Kaltenegger

#include <tuw_hardware_interface_template/generic_setup_prefix.h>

#include <algorithm>
#include <functional>
#include <string>

using tuw_hardware_interface::GenericSetupPrefix;

std::string GenericSetupPrefix::node_name_;
std::string GenericSetupPrefix::setup_name_;

void GenericSetupPrefix::setSetupName(const std::string& setup_name)
{
  GenericSetupPrefix::setup_name_ = setup_name;
}

std::string GenericSetupPrefix::getSetupName()
{
  if (GenericSetupPrefix::setup_name_.empty())
    ROS_WARN("the setup name was requested before is was set");

  return GenericSetupPrefix::setup_name_;
}

std::string GenericSetupPrefix::getNodeName()
{
  if (GenericSetupPrefix::node_name_.empty())
  {
    std::string node_name = ros::this_node::getNamespace() + ros::this_node::getName();
    // cut leading slash
    int cut_begin = 0;
    int cut_end = static_cast<int>(std::min(node_name.find_first_not_of('/'), node_name.size() - 1));
    GenericSetupPrefix::node_name_ = node_name.erase(cut_begin, cut_end);
  }
  return GenericSetupPrefix::node_name_;
}

std::string GenericSetupPrefix::getNodeNameLower()
{
  std::string node_name = GenericSetupPrefix::getNodeName();
  // to lower
  auto to_lower_begin = node_name.begin();
  auto to_lower_end = node_name.end();
  std::transform(to_lower_begin, to_lower_end, node_name.begin(), std::ptr_fun<int, int>(std::tolower));
  return node_name;
}

std::string GenericSetupPrefix::getNodeNameUpper()
{
  std::string node_name = GenericSetupPrefix::getNodeName();
  // to upper
  auto to_upper_begin = node_name.begin();
  auto to_upper_end = node_name.end();
  std::transform(to_upper_begin, to_upper_end, node_name.begin(), std::ptr_fun<int, int>(std::toupper));
  return node_name;
}

std::string GenericSetupPrefix::getLogPrefix()
{
  std::string node_name;
  node_name = ros::this_node::getNamespace() + ros::this_node::getName();
  // cut leading slash
  int cut_begin = 0;
  int cut_end = static_cast<int>(std::min(node_name.find_first_not_of('/'), node_name.size() - 1));
  node_name = node_name.erase(cut_begin, cut_end);

  if (GenericSetupPrefix::getSetupName().empty())
    return GenericSetupPrefix::getNodeNameUpper();
  else
    return GenericSetupPrefix::getNodeNameUpper() + "-" + GenericSetupPrefix::getSetupName();
}
