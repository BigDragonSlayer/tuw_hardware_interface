// Copyright 2022 Eugen Kaltenegger

/// Note this may have a problem: https://github.com/google/googletest/issues/930

#include <gtest/gtest.h>

#include <tuw_hardware_interface_trinamic/trinamic_connection/trinamic_message/trinamic_message.h>

using tuw_hardware_interface::TrinamicMessage;

TEST(TrinamicMessageTest, verifyChecksumSetVelocity)
{
  TrinamicMessage message(1,5,2,0,0);
  ASSERT_EQ(message.calculateChecksum(), 8);
}