// Copyright 2022 Eugen Kaltenegger

#include <gtest/gtest.h>

#include <tuw_hardware_interface_template/utils/file_loader.h>

#include <tuw_hardware_interface_template/description/generic_config_description.h>

#define TEST_FILE_PATH "/test/resources/description/generic_config_description_test.yaml"

using tuw_hardware_interface::FileLoader;
using tuw_hardware_interface::GenericConfigDescription;

class GenericConfigDescriptionTest : public ::testing::Test
{
protected:
  GenericConfigDescription generic_connection_description_ =
          GenericConfigDescription(FileLoader::loadYAMLFromFile(TEST_FILE_PATH));
};

TEST_F(GenericConfigDescriptionTest, verifyKeys)
{
  ASSERT_EQ(generic_connection_description_.getConfig().at(0).first, "c0_config_value");
  ASSERT_EQ(generic_connection_description_.getConfig().at(1).first, "b0_config_value");
  ASSERT_EQ(generic_connection_description_.getConfig().at(2).first, "a0_config_value");
  ASSERT_EQ(generic_connection_description_.getConfig().at(3).first, "a1_config_value");
  ASSERT_EQ(generic_connection_description_.getConfig().at(4).first, "b1_config_value");
  ASSERT_EQ(generic_connection_description_.getConfig().at(5).first, "c1_config_value");
}

TEST_F(GenericConfigDescriptionTest, verifyValues)
{
  ASSERT_EQ(generic_connection_description_.getConfig().at(0).second, 0);
  ASSERT_EQ(generic_connection_description_.getConfig().at(1).second, 1);
  ASSERT_EQ(generic_connection_description_.getConfig().at(2).second, 2);
  ASSERT_EQ(generic_connection_description_.getConfig().at(3).second, 2);
  ASSERT_EQ(generic_connection_description_.getConfig().at(4).second, 1);
  ASSERT_EQ(generic_connection_description_.getConfig().at(5).second, 0);
}

TEST_F(GenericConfigDescriptionTest, verifyLength)
{
  ASSERT_EQ(generic_connection_description_.getConfig().size(), 6);
}
