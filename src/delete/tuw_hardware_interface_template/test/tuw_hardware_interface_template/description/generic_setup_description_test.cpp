// Copyright 2022 Eugen Kaltenegger

#include <gtest/gtest.h>

#include <tuw_hardware_interface_template/utils/file_loader.h>

#include <tuw_hardware_interface_template/description/generic_setup_description.h>

#define TEST_FILE_PATH "/test/resources/description/generic_setup_description_test.yaml"

using tuw_hardware_interface::FileLoader;
using tuw_hardware_interface::GenericSetupDescription;

class GenericSetupDescriptionTest : public ::testing::Test
{
protected:
  GenericSetupDescription generic_setup_description_ =
          GenericSetupDescription(FileLoader::loadYAMLFromFile(TEST_FILE_PATH));
};

TEST_F(GenericSetupDescriptionTest, verifySetupName)
{
  ASSERT_EQ(generic_setup_description_.getName(), "setup_name");
}

TEST_F(GenericSetupDescriptionTest, verifyJointsNumber)
{
  ASSERT_EQ(generic_setup_description_.getJoints().size(), 3);
}
