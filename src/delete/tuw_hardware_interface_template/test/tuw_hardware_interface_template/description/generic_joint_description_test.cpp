// Copyright 2022 Eugen Kaltenegger

#include <gtest/gtest.h>

#include <tuw_hardware_interface_template/utils/file_loader.h>

#include <tuw_hardware_interface_template/description/generic_joint_description.h>

#define TEST_FILE_PATH "/test/resources/description/generic_joint_description_test.yaml"

using tuw_hardware_interface::FileLoader;
using tuw_hardware_interface::GenericJointDescription;

class GenericJointDescriptionTest : public ::testing::Test
{
protected:
  GenericJointDescription generic_joint_description_ =
          GenericJointDescription(FileLoader::loadYAMLFromFile(TEST_FILE_PATH));
};

TEST_F(GenericJointDescriptionTest, verifyName)
{
  ASSERT_EQ(generic_joint_description_.getName(), "joint");
}

TEST_F(GenericJointDescriptionTest, verifyId)
{
  ASSERT_EQ(generic_joint_description_.getId(), 0);
}

TEST_F(GenericJointDescriptionTest, verifyDiameter)
{
  ASSERT_EQ(generic_joint_description_.getDiameter(), 0.1);
}

TEST_F(GenericJointDescriptionTest, verifyConnectionDescriptionPointer)
{
  ASSERT_TRUE(generic_joint_description_.getConnectionDescription());
}

TEST_F(GenericJointDescriptionTest, verifyHardwareDescriptionPointer)
{
  ASSERT_TRUE(generic_joint_description_.getHardwareDescription());
}

TEST_F(GenericJointDescriptionTest, verifyConfigDescriptionPointer)
{
  ASSERT_TRUE(generic_joint_description_.getConfigDescription());
}
