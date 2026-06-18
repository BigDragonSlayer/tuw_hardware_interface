// Copyright 2022 Eugen Kaltenegger

/// Note this may have a problem: https://github.com/google/googletest/issues/930

#include <gtest/gtest.h>

#include "tuw_hardware_interface_trinamic/trinamic_connection/trinamic_message/trinamic_command.h"

using tuw_hardware_interface::TrinamicCommand;

TEST(TrinamicCommandTest, setVelocity0)
{
  TrinamicCommand command(1, 5, 2, 0, 0);
  ASSERT_EQ(command.toString(), "01 05 02 00 00 00 00 00 08");
}

TEST(TrinamicCommandTest, setVelocity10)
{
  TrinamicCommand command(1, 5, 2, 0, 10);
  ASSERT_EQ(command.toString(), "01 05 02 00 00 00 00 0A 12");
}

TEST(TrinamicCommandTest, setVelocity100)
{
  TrinamicCommand command(1, 5, 2, 0, 100);
  ASSERT_EQ(command.toString(), "01 05 02 00 00 00 00 64 6C");
}

TEST(TrinamicCommandTest, setVelocity1000)
{
  TrinamicCommand command(1, 5, 2, 0, 1000);
  ASSERT_EQ(command.toString(), "01 05 02 00 00 00 03 E8 F3");
}

TEST(TrinamicCommandTest, getVelocity)
{
  TrinamicCommand command(1, 6, 3, 0, 0);
  ASSERT_EQ(command.toString(), "01 06 03 00 00 00 00 00 0A");
}
