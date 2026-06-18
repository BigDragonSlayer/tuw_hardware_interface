// Copyright 2022 Eugen Kaltenegger

#include <gtest/gtest.h>

#include <tuw_hardware_interface_template/utils/file_loader.h>

#include <tuw_hardware_interface_template/description/generic_hardware_description.h>

#define TEST_FILE_PATH "/test/resources/description/generic_hardware_description_test.yaml"

using tuw_hardware_interface::FileLoader;
using tuw_hardware_interface::GenericHardwareDescription;

class GenericHardwareDescriptionTest : public ::testing::Test
{
protected:
  GenericHardwareDescription defined_resolution_generic_hardware_description_ =
          GenericHardwareDescription(FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["defined_resolution"]);
  GenericHardwareDescription undefined_resolution_generic_hardware_description_ =
          GenericHardwareDescription(FileLoader::loadYAMLFromFile(TEST_FILE_PATH)["undefined_resolution"]);
};

TEST_F(GenericHardwareDescriptionTest, verifyName)
{
  ASSERT_EQ(defined_resolution_generic_hardware_description_.getName(), "generic_hardware");
}

TEST_F(GenericHardwareDescriptionTest, verifyDefniedPositionResolution)
{
  ASSERT_TRUE(defined_resolution_generic_hardware_description_.getPositionResolution());
  ASSERT_EQ(*defined_resolution_generic_hardware_description_.getPositionResolution(), 0.5);
}

TEST_F(GenericHardwareDescriptionTest, verifyyDefniedVelocityResolution)
{
  ASSERT_TRUE(defined_resolution_generic_hardware_description_.getPositionResolution());
  ASSERT_EQ(*defined_resolution_generic_hardware_description_.getPositionResolution(), 0.5);
}

TEST_F(GenericHardwareDescriptionTest, verifyyDefniedEffortResolution)
{
  ASSERT_TRUE(defined_resolution_generic_hardware_description_.getPositionResolution());
  ASSERT_EQ(*defined_resolution_generic_hardware_description_.getPositionResolution(), 0.5);
}

TEST_F(GenericHardwareDescriptionTest, verifyUndefinedPositionResolution)
{
  ASSERT_FALSE(undefined_resolution_generic_hardware_description_.getPositionResolution());
}

TEST_F(GenericHardwareDescriptionTest, verifyUndefinedVelocityResolution)
{
  ASSERT_FALSE(undefined_resolution_generic_hardware_description_.getPositionResolution());
}

TEST_F(GenericHardwareDescriptionTest, verifyUndefinedEffortResolution)
{
  ASSERT_FALSE(undefined_resolution_generic_hardware_description_.getPositionResolution());
}

TEST_F(GenericHardwareDescriptionTest, verifyTargetPointer)
{
  ASSERT_TRUE(defined_resolution_generic_hardware_description_.getTargetIdentifierToDescription());
}

TEST_F(GenericHardwareDescriptionTest, verifyTargetIdentifier)
{
  ASSERT_EQ(defined_resolution_generic_hardware_description_.getTargetIdentifierToDescription()->size(), 3);
}

TEST_F(GenericHardwareDescriptionTest, verifyActualPointer)
{
  ASSERT_TRUE(defined_resolution_generic_hardware_description_.getActualIdentifierToDescription());
}

TEST_F(GenericHardwareDescriptionTest, verifyActualIdentifier)
{
  ASSERT_EQ(defined_resolution_generic_hardware_description_.getActualIdentifierToDescription()->size(), 3);
}

TEST_F(GenericHardwareDescriptionTest, verifyConfigIdentifierToDescripionPointer)
{
  ASSERT_TRUE(defined_resolution_generic_hardware_description_.getConfigIdentifierToDescription());
}

TEST_F(GenericHardwareDescriptionTest, verifyConfigIdentifierToDescripionSize)
{
  ASSERT_EQ(defined_resolution_generic_hardware_description_.getConfigIdentifierToDescription()->size(), 2);
}

TEST_F(GenericHardwareDescriptionTest, verifyConfigIdentifiersPointer)
{
  ASSERT_TRUE(defined_resolution_generic_hardware_description_.getConfigIdentifiers());
}

TEST_F(GenericHardwareDescriptionTest, verifyConfigIdentifiersSize)
{
  ASSERT_EQ(defined_resolution_generic_hardware_description_.getConfigIdentifiers()->size(), 2);
}
