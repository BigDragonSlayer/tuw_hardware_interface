// Copyright 2022 Eugen Kaltenegger

#include <gtest/gtest.h>

#include <tuw_hardware_interface_template/utils/file_loader.h>

#include <tuw_hardware_interface_template/description/generic_hardware_parameter_description.h>
#include <tuw_hardware_interface_template/generic_hardware_parameter.h>

#define TEST_FILE_PATH "/test/resources/generic_hardware_parameter_test.yaml"

using tuw_hardware_interface::FileLoader;
using tuw_hardware_interface::GenericHardwareParameter;
using tuw_hardware_interface::GenericHardwareParameterDescription;

class GenericHardwareParameterTest : public ::testing::Test
{
protected:
  GenericHardwareParameter target_ =
          GenericHardwareParameter(
                  GenericHardwareParameterDescription(
                          FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["target_state_parameters"]));
  GenericHardwareParameter actual_ =
          GenericHardwareParameter(
                  GenericHardwareParameterDescription(
                          FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["actual_state_parameters"]));
  GenericHardwareParameter enum_ =
          GenericHardwareParameter(
                  GenericHardwareParameterDescription(
                          FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["config_parameters"]["enum"]));
  GenericHardwareParameter range_ =
          GenericHardwareParameter(
                  GenericHardwareParameterDescription(
                          FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["config_parameters"]["range"]));
};

TEST_F(GenericHardwareParameterTest, verifyTargetPointers)
{
  ASSERT_TRUE(target_.getIdentifier());
  ASSERT_TRUE(target_.getAddress());
  ASSERT_TRUE(target_.getLength());
  ASSERT_TRUE(target_.getRange());
  ASSERT_FALSE(target_.getEnum());
}

TEST_F(GenericHardwareParameterTest, verfiyTargetValues)
{
  ASSERT_EQ(*target_.getIdentifier(), "tsp");
  ASSERT_EQ(*target_.getAddress(), 1);
  ASSERT_EQ(*target_.getLength(), 1);
  ASSERT_EQ(target_.getRange()->at("min"), -1);
  ASSERT_EQ(target_.getRange()->at("max"), 1);
}

TEST_F(GenericHardwareParameterTest, verifyTargetType)
{
  ASSERT_EQ(target_.getType(), GenericHardwareParameter::Type::TARGET);
}

TEST_F(GenericHardwareParameterTest, verifyActualPointers)
{
  ASSERT_TRUE(actual_.getIdentifier());
  ASSERT_TRUE(actual_.getAddress());
  ASSERT_TRUE(actual_.getLength());
  ASSERT_FALSE(actual_.getDescription());
  ASSERT_FALSE(actual_.getRange());
}

TEST_F(GenericHardwareParameterTest, verifyActualValues)
{
  ASSERT_EQ(*actual_.getIdentifier(), "asp");
  ASSERT_EQ(*actual_.getAddress(), 1);
  ASSERT_EQ(*actual_.getLength(), 1);
}

TEST_F(GenericHardwareParameterTest, verifyActualType)
{
  ASSERT_EQ(actual_.getType(), GenericHardwareParameter::Type::ACTUAL);
}

TEST_F(GenericHardwareParameterTest, verifyEnumPointers)
{
  ASSERT_TRUE(enum_.getIdentifier());
  ASSERT_TRUE(enum_.getAddress());
  ASSERT_TRUE(enum_.getLength());
  ASSERT_TRUE(enum_.getEnum());
  ASSERT_FALSE(enum_.getRange());
}

TEST_F(GenericHardwareParameterTest, verifyEnumValues)
{
  ASSERT_EQ(*enum_.getIdentifier(), "ecp");
  ASSERT_EQ(*enum_.getDescription(), "ecp");
  ASSERT_EQ(*enum_.getAddress(), 1);
  ASSERT_EQ(*enum_.getLength(), 1);
  ASSERT_EQ(enum_.getEnum()->at("a"), 0);
  ASSERT_EQ(enum_.getEnum()->at("b"), 1);
  ASSERT_EQ(enum_.getEnum()->at("c"), 2);
}

TEST_F(GenericHardwareParameterTest, verifyEnumType)
{
  ASSERT_EQ(enum_.getType(), GenericHardwareParameter::Type::ENUM);
}

TEST_F(GenericHardwareParameterTest, verifyRangePointers)
{
  ASSERT_TRUE(range_.getIdentifier());
  ASSERT_TRUE(range_.getAddress());
  ASSERT_TRUE(range_.getLength());
  ASSERT_TRUE(range_.getRange());
  ASSERT_FALSE(range_.getEnum());
}

TEST_F(GenericHardwareParameterTest, verifyRangeValues)
{
  ASSERT_EQ(*range_.getIdentifier(), "rcp");
  ASSERT_EQ(*range_.getDescription(), "rcp");
  ASSERT_EQ(*range_.getAddress(), 1);
  ASSERT_EQ(*range_.getLength(), 1);
  ASSERT_EQ(range_.getRange()->at("min"), -1);
  ASSERT_EQ(range_.getRange()->at("max"), 1);
}

TEST_F(GenericHardwareParameterTest, verifyRangeType)
{
  ASSERT_EQ(range_.getType(), GenericHardwareParameter::Type::RANGE);
}

TEST_F(GenericHardwareParameterTest, verifyEqualsOperator)
{
  ASSERT_TRUE(target_ == target_);
  ASSERT_TRUE(actual_ == actual_);
  ASSERT_TRUE(enum_ == enum_);
  ASSERT_TRUE(range_ == range_);
  ASSERT_FALSE(target_ == actual_);
}
