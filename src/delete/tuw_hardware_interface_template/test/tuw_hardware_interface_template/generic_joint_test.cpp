// Copyright 2022 Eugen Kaltenegger

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <tuw_hardware_interface_template/utils/file_loader.h>

#include <memory>
#include <string>

#include <tuw_hardware_interface_template/description/generic_hardware_description.h>
#include <tuw_hardware_interface_template/description/generic_joint_description.h>
#include <tuw_hardware_interface_template/generic_config.h>
#include <tuw_hardware_interface_template/generic_connection.h>
#include <tuw_hardware_interface_template/generic_hardware.h>
#include <tuw_hardware_interface_template/generic_joint.h>
#include <tuw_hardware_interface_template/generic_setup_prefix.h>

#define URDF_FILE_PATH "/test/resources/urdf.xml"
#define TEST_FILE_PATH "/test/resources/generic_joint_test.yaml"

using tuw_hardware_interface::FileLoader;
using tuw_hardware_interface::GenericConfig;
using tuw_hardware_interface::GenericConnection;
using tuw_hardware_interface::GenericConfigDescription;
using tuw_hardware_interface::GenericHardware;
using tuw_hardware_interface::GenericHardwareDescription;
using tuw_hardware_interface::GenericHardwareParameter;
using tuw_hardware_interface::GenericHardwareParameterDescription;
using tuw_hardware_interface::GenericJoint;
using tuw_hardware_interface::GenericJointDescription;
using tuw_hardware_interface::GenericSetupPrefix;

class ConnectionMock : public GenericConnection
{
public:
  ConnectionMock() = default;

  bool connect() override
  { return true; };

  bool disconnect() override
  { return true; };
  MOCK_METHOD(void, write, (int id, GenericHardwareParameter hardware_parameter, int data), (override));
  MOCK_METHOD(int, read, (int id, GenericHardwareParameter hardware_parameter), (override));
  MOCK_METHOD(void, read, (int id, (std::vector<std::pair<GenericHardwareParameter, int*>> parameter_data_pairs)), (override));
};

class HardwareMock : public GenericHardware
{
public:
  HardwareMock() = default;

  std::string getName() override
  { return "hardware_mock"; };

  bool supportsActualMode(Mode mode) override
  { return true; };

  bool supportsTargetMode(Mode mode) override
  { return true; };

  int convertToHardwareResolution(double input, Mode mode) override
  { return static_cast<int>(input); };

  double convertFromHardwareResolution(int input, Mode mode) override
  { return static_cast<double>(input); };

  GenericHardwareParameter getTargetParameterForMode(Mode mode) override
  { return this->target_state_parameter_; };

  GenericHardwareParameter getActualParameterForMode(Mode mode) override
  { return this->actual_state_parameter_; };

  GenericHardwareParameter target_state_parameter_ =
          GenericHardwareParameter(
                  GenericHardwareParameterDescription(
                          FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["target_state_parameters"]));
  GenericHardwareParameter actual_state_parameter_ =
          GenericHardwareParameter(
                  GenericHardwareParameterDescription(
                          FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["actual_state_parameters"]));
};

class ConfigMock : public GenericConfig
{
public:
  ConfigMock() = default;
};

class GenericJointTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    GenericSetupPrefix::setSetupName("test_setup");
    ros::param::set("/robot_description", FileLoader::loadURDFFromFile(URDF_FILE_PATH));
    this->joint_ = std::make_unique<GenericJoint>(this->joint_description_);
    this->joint_->setConnection(this->connection_);
    this->joint_->setHardware(this->hardware_);
    this->joint_->setConfig(this->config_);
  }

  std::unique_ptr<GenericJoint> joint_;

  std::shared_ptr<ConnectionMock> connection_ = std::make_shared<ConnectionMock>();
  std::shared_ptr<HardwareMock> hardware_ = std::make_shared<HardwareMock>();
  std::shared_ptr<ConfigMock> config_ = std::make_shared<ConfigMock>();

  GenericJointDescription joint_description_ =
          GenericJointDescription(
                  FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["joint"]);
};

TEST_F(GenericJointTest, verifyName)
{
  ASSERT_EQ(this->joint_->getName(), "test_joint");
}

TEST_F(GenericJointTest, verifyId)
{
  ASSERT_EQ(this->joint_->getId(), 0);
}

TEST_F(GenericJointTest, verifyTargetPosition)
{
  EXPECT_CALL(*this->connection_, write(0, hardware_->target_state_parameter_, 0)).Times(1);
  this->joint_->setMode(GenericHardware::Mode::POSITION);
  this->joint_->write(ros::Duration(1));
}

TEST_F(GenericJointTest, verifyTargetVelocoty)
{
  EXPECT_CALL(*this->connection_, write(0, hardware_->target_state_parameter_, 0)).Times(1);
  this->joint_->setMode(GenericHardware::Mode::VELOCITY);
  this->joint_->write(ros::Duration(1));
}

TEST_F(GenericJointTest, verifyTargetEffort)
{
  EXPECT_CALL(*this->connection_, write(0, hardware_->target_state_parameter_, 0)).Times(1);
  this->joint_->setMode(GenericHardware::Mode::EFFORT);
  this->joint_->write(ros::Duration(1));
}

TEST_F(GenericJointTest, verifyActual)
{
  std::vector<std::pair<GenericHardwareParameter, int*>> vector;
  EXPECT_CALL(*this->connection_, read(0, vector)).Times(1);
  this->joint_->setMode(GenericHardware::Mode::POSITION);
  this->joint_->read(ros::Duration(1));
}

TEST_F(GenericJointTest, verifyWrtieForwarding)
{
  EXPECT_CALL(*this->connection_, write(0, this->hardware_->actual_state_parameter_, 0)).Times(1);
  this->joint_->write(this->hardware_->actual_state_parameter_, 0);
}

TEST_F(GenericJointTest, verifyReadForwarding)
{
  EXPECT_CALL(*this->connection_, read(0, this->hardware_->actual_state_parameter_)).Times(1);
  this->joint_->read(this->hardware_->actual_state_parameter_);
}

TEST_F(GenericJointTest, verifyJointStateHandle)
{
  ASSERT_TRUE(this->joint_->getJointStateHandle());
}

TEST_F(GenericJointTest, verifyJointPositionHandle)
{
  ASSERT_TRUE(this->joint_->getJointPositionHandle());
}

TEST_F(GenericJointTest, verifyJointVelocityHandle)
{
  ASSERT_TRUE(this->joint_->getJointVelocityHandle());
}

TEST_F(GenericJointTest, verifyJointEffortHandle)
{
  ASSERT_TRUE(this->joint_->getJointEffortHandle());
}

TEST_F(GenericJointTest, verifyMPStoRadPS)
{
  ASSERT_EQ(this->joint_->MPStoRadPS(1.0), 10.0);
}

TEST_F(GenericJointTest, verifyRadPStoMPS)
{
  ASSERT_EQ(this->joint_->RadPStoMPS(10.0), 1.0);
}
