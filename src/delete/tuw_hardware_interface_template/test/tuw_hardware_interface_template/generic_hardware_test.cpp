// Copyright 2022 Eugen Kaltenegger

#include <gtest/gtest.h>

#include <tuw_hardware_interface_template/utils/file_loader.h>

#include <memory>

#include <tuw_hardware_interface_template/generic_hardware.h>
#include <tuw_hardware_interface_template/generic_setup_prefix.h>

using tuw_hardware_interface::FileLoader;
using tuw_hardware_interface::GenericHardware;
using tuw_hardware_interface::GenericHardwareDescription;
using tuw_hardware_interface::GenericSetupPrefix;

#define TEST_FILE_PATH "/test/resources/generic_hardware_test.yaml"

class GenericHardwareTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    GenericSetupPrefix::setSetupName("test_setup");
    ROS_INFO("LOG INFO/WARNING/ERROR BELOW CAN BE IGNORED IN TESTS!");
    complete_generic_hardware_ = GenericHardware::getHardware(complete_generic_hardware_description_);
    incomplete_generic_hardware_ = GenericHardware::getHardware(incomplete_generic_hardware_description_);
  }

  GenericHardwareDescription complete_generic_hardware_description_ =
          GenericHardwareDescription(FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["complete"]);
  GenericHardwareDescription incomplete_generic_hardware_description_ =
          GenericHardwareDescription(FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["incomplete"]);
  std::shared_ptr<GenericHardware> complete_generic_hardware_;
  std::shared_ptr<GenericHardware> incomplete_generic_hardware_;
};

TEST_F(GenericHardwareTest, verifPointer)
{
  ASSERT_TRUE(GenericHardware::getHardware(complete_generic_hardware_description_));
}

TEST_F(GenericHardwareTest, verifyEqualPointers)
{
  ASSERT_EQ(GenericHardware::getHardware(complete_generic_hardware_description_),
            GenericHardware::getHardware(complete_generic_hardware_description_));
}

TEST_F(GenericHardwareTest, verifyNotEqualPointers)
{
  ASSERT_NE(GenericHardware::getHardware(complete_generic_hardware_description_),
            GenericHardware::getHardware(incomplete_generic_hardware_description_));
}

TEST_F(GenericHardwareTest, verifyName)
{
  ASSERT_EQ(complete_generic_hardware_->getName(), "complete_generic_hardware");
}

TEST_F(GenericHardwareTest, verifyPositionConvertionTo)
{
  ASSERT_EQ(complete_generic_hardware_->convertToHardwareResolution(0.5, GenericHardware::Mode::POSITION), 1);
}

TEST_F(GenericHardwareTest, verifyPositionConvertionFrom)
{
  ASSERT_EQ(complete_generic_hardware_->convertFromHardwareResolution(1, GenericHardware::Mode::POSITION), 0.5);
}

TEST_F(GenericHardwareTest, verifyVelocityConvertionTo)
{
  ASSERT_EQ(complete_generic_hardware_->convertToHardwareResolution(0.5, GenericHardware::Mode::VELOCITY), 1);
}

TEST_F(GenericHardwareTest, verifyVelocityConvertionFrom)
{
  ASSERT_EQ(complete_generic_hardware_->convertFromHardwareResolution(1, GenericHardware::Mode::VELOCITY), 0.5);
}

TEST_F(GenericHardwareTest, verifyEffortConvertionTo)
{
  ASSERT_EQ(complete_generic_hardware_->convertToHardwareResolution(0.5, GenericHardware::Mode::EFFORT), 1);
}

TEST_F(GenericHardwareTest, verifyEffortConvertionFrom)
{
  ASSERT_EQ(complete_generic_hardware_->convertFromHardwareResolution(1, GenericHardware::Mode::EFFORT), 0.5);
}

TEST_F(GenericHardwareTest, verifySupportedTargetModes)
{
  ASSERT_TRUE(complete_generic_hardware_->supportsTargetMode(GenericHardware::Mode::POSITION));
  ASSERT_TRUE(complete_generic_hardware_->supportsTargetMode(GenericHardware::Mode::VELOCITY));
  ASSERT_TRUE(complete_generic_hardware_->supportsTargetMode(GenericHardware::Mode::EFFORT));
}

TEST_F(GenericHardwareTest, verifySupportedTargetIdentifiers)
{
  ASSERT_EQ(*complete_generic_hardware_->
          getTargetParameterForMode(GenericHardware::Mode::POSITION).getIdentifier(), "target_position");
  ASSERT_EQ(*complete_generic_hardware_->
          getTargetParameterForMode(GenericHardware::Mode::VELOCITY).getIdentifier(), "target_velocity");
  ASSERT_EQ(*complete_generic_hardware_->
          getTargetParameterForMode(GenericHardware::Mode::EFFORT).getIdentifier(), "target_effort");
}

TEST_F(GenericHardwareTest, verifySupportedActualModes)
{
  ASSERT_TRUE(complete_generic_hardware_->supportsActualMode(GenericHardware::Mode::POSITION));
  ASSERT_TRUE(complete_generic_hardware_->supportsActualMode(GenericHardware::Mode::VELOCITY));
  ASSERT_TRUE(complete_generic_hardware_->supportsActualMode(GenericHardware::Mode::EFFORT));
}

TEST_F(GenericHardwareTest, verifySupportedActualIdentifiers)
{
  ASSERT_EQ(*complete_generic_hardware_->
          getActualParameterForMode(GenericHardware::Mode::POSITION).getIdentifier(), "actual_position");
  ASSERT_EQ(*complete_generic_hardware_->
          getActualParameterForMode(GenericHardware::Mode::VELOCITY).getIdentifier(), "actual_velocity");
  ASSERT_EQ(*complete_generic_hardware_->
          getActualParameterForMode(GenericHardware::Mode::EFFORT).getIdentifier(), "actual_effort");
}

TEST_F(GenericHardwareTest, verifyUnsupportedTargetModes)
{
  ASSERT_FALSE(incomplete_generic_hardware_->supportsTargetMode(GenericHardware::Mode::POSITION));
  ASSERT_FALSE(incomplete_generic_hardware_->supportsTargetMode(GenericHardware::Mode::VELOCITY));
  ASSERT_FALSE(incomplete_generic_hardware_->supportsTargetMode(GenericHardware::Mode::EFFORT));
}

TEST_F(GenericHardwareTest, verifyConfigParametersPointer)
{
  ASSERT_TRUE(complete_generic_hardware_->getConfigIdentifiers());
}

TEST_F(GenericHardwareTest, verifyConfigParametersSize)
{
  ASSERT_EQ(complete_generic_hardware_->getConfigIdentifiers()->size(), 2);
}

TEST_F(GenericHardwareTest, verifyConfigIdentifierToParameterPointer)
{
  ASSERT_TRUE(complete_generic_hardware_->getConfigIdentifierToParameter());
}

TEST_F(GenericHardwareTest, verifyConfigIdentifierToParameterValues)
{
  ASSERT_EQ(*complete_generic_hardware_->getConfigIdentifierToParameter()->at("ecp").getIdentifier(), "ecp");
  ASSERT_EQ(*complete_generic_hardware_->getConfigIdentifierToParameter()->at("rcp").getIdentifier(), "rcp");
}

TEST_F(GenericHardwareTest, PositionToString)
{
  ASSERT_EQ(GenericHardware::modeToString(GenericHardware::Mode::POSITION), "POSITION");
}

TEST_F(GenericHardwareTest, PositionFromString)
{
  ASSERT_EQ(GenericHardware::modeFromString("POSITION"), GenericHardware::Mode::POSITION);
  ASSERT_EQ(GenericHardware::modeFromString("_POSITION_"), GenericHardware::Mode::POSITION);
}

TEST_F(GenericHardwareTest, VelocityToString)
{
  ASSERT_EQ(GenericHardware::modeToString(GenericHardware::Mode::VELOCITY), "VELOCITY");
}

TEST_F(GenericHardwareTest, VelocityFromString)
{
  ASSERT_EQ(GenericHardware::modeFromString("VELOCITY"), GenericHardware::Mode::VELOCITY);
  ASSERT_EQ(GenericHardware::modeFromString("_VELOCITY_"), GenericHardware::Mode::VELOCITY);
}

TEST_F(GenericHardwareTest, EffortToString)
{
  ASSERT_EQ(GenericHardware::modeToString(GenericHardware::Mode::EFFORT), "EFFORT");
}

TEST_F(GenericHardwareTest, EffortFromString)
{
  ASSERT_EQ(GenericHardware::modeFromString("EFFORT"), GenericHardware::Mode::EFFORT);
  ASSERT_EQ(GenericHardware::modeFromString("_EFFORT_"), GenericHardware::Mode::EFFORT);
}
