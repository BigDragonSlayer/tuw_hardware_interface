// Copyright 2022 Eugen Kaltenegger

#include <gtest/gtest.h>

#include <tuw_hardware_interface_template/utils/file_loader.h>

#include <tuw_hardware_interface_template/description/generic_connection_description.h>

#define TEST_FILE_PATH "/test/resources/description/generic_connection_description_test.yaml"

using tuw_hardware_interface::FileLoader;
using tuw_hardware_interface::GenericConnectionDescription;

class GenericConnectionDescriptionTest : public ::testing::Test
{
protected:
  GenericConnectionDescription generic_connection_description_ =
          GenericConnectionDescription(FileLoader::loadYAMLFromFile(TEST_FILE_PATH));
};

TEST_F(GenericConnectionDescriptionTest, verifyHash)
{
  ASSERT_EQ(generic_connection_description_.getHash(), "/dev/ttyUSB0_57600");
}

TEST_F(GenericConnectionDescriptionTest, verifyPort)
{
  ASSERT_EQ(generic_connection_description_.getPort(), "/dev/ttyUSB0");
}

TEST_F(GenericConnectionDescriptionTest, verifyBaudrate)
{
  ASSERT_EQ(generic_connection_description_.getBaudrate(), 57600);
}
