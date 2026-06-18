// Copyright 2022 Eugen Kaltenegger

#include <gtest/gtest.h>

#include <tuw_hardware_interface_template/utils/file_loader.h>

#include <tuw_hardware_interface_template/description/generic_hardware_parameter_description.h>

#define TEST_FILE_PATH "/test/resources/description/generic_hardware_parameter_description_test.yaml"

using tuw_hardware_interface::FileLoader;
using tuw_hardware_interface::GenericHardwareParameterDescription;

class GenericHardwareParameterDescriptionTest : public ::testing::Test
{
protected:
  GenericHardwareParameterDescription target_ =
          GenericHardwareParameterDescription(
                  FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["target_state_parameters"]);
  GenericHardwareParameterDescription actual_ =
          GenericHardwareParameterDescription(
                  FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["actual_state_parameters"]);

  GenericHardwareParameterDescription enum_ =
          GenericHardwareParameterDescription(
                  FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["config_parameters"]["enum"]);
  GenericHardwareParameterDescription enum_null_ =
          GenericHardwareParameterDescription(
                  FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["config_parameters"]["enum_null"]);

  GenericHardwareParameterDescription range_ =
          GenericHardwareParameterDescription(
                  FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["config_parameters"]["range"]);
  GenericHardwareParameterDescription range_null_ =
          GenericHardwareParameterDescription(
                  FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["config_parameters"]["range_null"]);
};

TEST_F(GenericHardwareParameterDescriptionTest, verifyTargetPointers)
{
  ASSERT_TRUE(target_.getIdentifier());
  ASSERT_TRUE(target_.getAddress());
  ASSERT_TRUE(target_.getLength());
  ASSERT_TRUE(target_.getRange());
  ASSERT_FALSE(target_.getEnum());
}

TEST_F(GenericHardwareParameterDescriptionTest, verifyTargetValues)
{
  ASSERT_EQ(*target_.getIdentifier(), "tsp");
  ASSERT_EQ(*target_.getAddress(), 1);
  ASSERT_EQ(*target_.getLength(), 1);
  ASSERT_EQ(target_.getRange()->at("min"), -1);
  ASSERT_EQ(target_.getRange()->at("max"), 1);
}

TEST_F(GenericHardwareParameterDescriptionTest, verifyActualPointers)
{
  ASSERT_TRUE(actual_.getIdentifier());
  ASSERT_TRUE(actual_.getAddress());
  ASSERT_TRUE(actual_.getLength());
  ASSERT_FALSE(actual_.getRange());
  ASSERT_FALSE(actual_.getEnum());
}

TEST_F(GenericHardwareParameterDescriptionTest, verifyActualValues)
{
  ASSERT_EQ(*actual_.getIdentifier(), "asp");
  ASSERT_EQ(*actual_.getAddress(), 1);
  ASSERT_EQ(*actual_.getLength(), 1);
}

TEST_F(GenericHardwareParameterDescriptionTest, verifyEnumPointers)
{
  ASSERT_TRUE(enum_.getIdentifier());
  ASSERT_TRUE(enum_.getAddress());
  ASSERT_TRUE(enum_.getLength());
  ASSERT_TRUE(enum_.getEnum());
  ASSERT_FALSE(enum_.getRange());
}

TEST_F(GenericHardwareParameterDescriptionTest, verifyEnumNullPointers)
{
  ASSERT_TRUE(enum_null_.getIdentifier());
  ASSERT_TRUE(enum_null_.getAddress());
  ASSERT_TRUE(enum_null_.getLength());
  ASSERT_TRUE(enum_null_.getEnum());
  ASSERT_FALSE(enum_null_.getRange());
}

TEST_F(GenericHardwareParameterDescriptionTest, verifyEnumValues)
{
  ASSERT_EQ(*enum_.getIdentifier(), "ecp");
  ASSERT_EQ(*enum_.getDescription(), "ecp");
  ASSERT_EQ(*enum_.getAddress(), 1);
  ASSERT_EQ(*enum_.getLength(), 1);
  ASSERT_EQ(enum_.getEnum()->at("a"), 0);
  ASSERT_EQ(enum_.getEnum()->at("b"), 1);
  ASSERT_EQ(enum_.getEnum()->at("c"), 2);
}

TEST_F(GenericHardwareParameterDescriptionTest, verifyRangePointers)
{
  ASSERT_TRUE(range_.getIdentifier());
  ASSERT_TRUE(range_.getAddress());
  ASSERT_TRUE(range_.getLength());
  ASSERT_TRUE(range_.getRange());
  ASSERT_FALSE(range_.getEnum());
}

TEST_F(GenericHardwareParameterDescriptionTest, verifyNullPointers)
{
  ASSERT_TRUE(range_null_.getIdentifier());
  ASSERT_TRUE(range_null_.getAddress());
  ASSERT_TRUE(range_null_.getLength());
  ASSERT_TRUE(range_null_.getRange());
  ASSERT_FALSE(range_null_.getEnum());
}

TEST_F(GenericHardwareParameterDescriptionTest, verifyRangeValues)
{
  ASSERT_EQ(*range_.getIdentifier(), "rcp");
  ASSERT_EQ(*range_.getDescription(), "rcp");
  ASSERT_EQ(*range_.getAddress(), 1);
  ASSERT_EQ(*range_.getLength(), 1);
  ASSERT_EQ(range_.getRange()->at("min"), -1);
  ASSERT_EQ(range_.getRange()->at("max"), 1);
}
