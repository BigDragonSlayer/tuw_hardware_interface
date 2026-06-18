// Copyright 2022 Eugen Kaltenegger

#include <gtest/gtest.h>

#include <string>

#include <tuw_hardware_interface_template/generic_setup_prefix.h>

using tuw_hardware_interface::GenericSetupPrefix;

TEST(GenericSetupNameTest, verifySetupName)
{
  std::string name = "test_name";
  GenericSetupPrefix::setSetupName(name);
  ASSERT_EQ(GenericSetupPrefix::getSetupName(), name);
}

TEST(GenericSetupNameTest, verifyNodeName)
{
  ASSERT_EQ(GenericSetupPrefix::getNodeName(), "tuw_ros_control_generic_unit_test");
}

TEST(GenericSetupNameTest, verifyNodeNameLower)
{
  ASSERT_EQ(GenericSetupPrefix::getNodeNameLower(), "tuw_ros_control_generic_unit_test");
}

TEST(GenericSetupNameTest, verifyNodeNameUpper)
{
  ASSERT_EQ(GenericSetupPrefix::getNodeNameUpper(), "TUW_ROS_CONTROL_GENERIC_UNIT_TEST");
}

TEST(GenericSetupPrefix, verifyLogPrefix)
{
  std::string name = "test_name";
  GenericSetupPrefix::setSetupName(name);
  ASSERT_EQ(GenericSetupPrefix::getLogPrefix(), "TUW_ROS_CONTROL_GENERIC_UNIT_TEST-test_name");
}
