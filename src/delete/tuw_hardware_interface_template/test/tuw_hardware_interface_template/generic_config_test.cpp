// Copyright 2022 Eugen Kaltenegger

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <tuw_hardware_interface_template/utils/file_loader.h>

#include <memory>
#include <string>

#include <tuw_hardware_interface_template/description/generic_hardware_description.h>
#include <tuw_hardware_interface_template/generic_config.h>
#include <tuw_hardware_interface_template/generic_hardware.h>
#include <tuw_hardware_interface_template/generic_joint.h>
#include <tuw_hardware_interface_template/generic_setup_prefix.h>

#define TEST_FILE_PATH "/test/resources/generic_config_test.yaml"

using tuw_hardware_interface::FileLoader;
using tuw_hardware_interface::GenericConfig;
using tuw_hardware_interface::GenericConfigDescription;
using tuw_hardware_interface::GenericHardware;
using tuw_hardware_interface::GenericHardwareDescription;
using tuw_hardware_interface::GenericHardwareParameter;
using tuw_hardware_interface::GenericHardwareParameterDescription;
using tuw_hardware_interface::GenericJoint;
using tuw_hardware_interface::GenericSetupPrefix;

class JointMock : public GenericJoint
{
public:
  JointMock() = default;
  MOCK_METHOD(std::string, getName, (), (override));
  MOCK_METHOD(void, write, (GenericHardwareParameter hardware_parameter, int data), (override));
  MOCK_METHOD(int, read, (GenericHardwareParameter hardware_parameter), (override));
};

class GenericConfigTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    GenericSetupPrefix::setSetupName("test_setup");
    EXPECT_CALL(*mock_joint_, getName()).WillRepeatedly(testing::Return("test_joint"));
  }

  std::shared_ptr<JointMock> mock_joint_ = std::make_shared<JointMock>();
  std::shared_ptr<GenericHardware> generic_hardware_ =
          GenericHardware::getHardware(
                  GenericHardwareDescription(FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["hardware"]));
  GenericConfigDescription generic_config_description_ =
          GenericConfigDescription(FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["config"]);
};

TEST_F(GenericConfigTest, verifyConstructorWithoutConfig)
{
  EXPECT_CALL(*mock_joint_, read(generic_hardware_->getConfigIdentifierToParameter()->at("ecp")))
          .Times(1).WillRepeatedly(testing::Return(1));
  EXPECT_CALL(*mock_joint_, read(generic_hardware_->getConfigIdentifierToParameter()->at("rcp")))
          .Times(1).WillRepeatedly(testing::Return(0));

  GenericConfig generic_config(mock_joint_, generic_hardware_);
}

TEST_F(GenericConfigTest, verifyServiceWithoutConfig)
{
  EXPECT_CALL(*mock_joint_, read(generic_hardware_->getConfigIdentifierToParameter()->at("ecp"))).Times(
          1).WillRepeatedly(testing::Return(1));
  EXPECT_CALL(*mock_joint_, read(generic_hardware_->getConfigIdentifierToParameter()->at("rcp"))).Times(
          1).WillRepeatedly(testing::Return(0));

  GenericConfig generic_config(mock_joint_, generic_hardware_);

  EXPECT_TRUE(ros::service::waitForService("tuw_ros_control_generic_unit_test/test_joint/set_parameters", true));
  ros::Duration(0.01).sleep();  // required to supress warning about closed service
}

TEST_F(GenericConfigTest, verifyConstructorWithConfig)
{
  EXPECT_CALL(*mock_joint_, write(generic_hardware_->getConfigIdentifierToParameter()->at("ecp"), testing::_)).Times(1);
  EXPECT_CALL(*mock_joint_, write(generic_hardware_->getConfigIdentifierToParameter()->at("rcp"), testing::_)).Times(1);
  EXPECT_CALL(*mock_joint_, read(generic_hardware_->getConfigIdentifierToParameter()->at("ecp"))).Times(
          2).WillRepeatedly(testing::Return(1));
  EXPECT_CALL(*mock_joint_, read(generic_hardware_->getConfigIdentifierToParameter()->at("rcp"))).Times(
          2).WillRepeatedly(testing::Return(0));

  ROS_INFO("LOG INFO/WARNING/ERROR BELOW CAN BE IGNORED IN TESTS!");

  GenericConfig generic_config(mock_joint_, generic_hardware_, generic_config_description_);
}

TEST_F(GenericConfigTest, verifyServiceWithConfig)
{
  EXPECT_CALL(*mock_joint_, write(generic_hardware_->getConfigIdentifierToParameter()->at("ecp"), testing::_)).Times(1);
  EXPECT_CALL(*mock_joint_, write(generic_hardware_->getConfigIdentifierToParameter()->at("rcp"), testing::_)).Times(1);
  EXPECT_CALL(*mock_joint_, read(generic_hardware_->getConfigIdentifierToParameter()->at("ecp"))).Times(
          2).WillRepeatedly(testing::Return(1));
  EXPECT_CALL(*mock_joint_, read(generic_hardware_->getConfigIdentifierToParameter()->at("rcp"))).Times(
          2).WillRepeatedly(testing::Return(0));

  ROS_INFO("LOG INFO/WARNING/ERROR BELOW CAN BE IGNORED IN TESTS!");

  GenericConfig generic_config(mock_joint_, generic_hardware_, generic_config_description_);

  EXPECT_TRUE(ros::service::exists("tuw_ros_control_generic_unit_test/test_joint/set_parameters", false));
  ros::Duration(0.01).sleep();  // required to supress warning about closed service
}
