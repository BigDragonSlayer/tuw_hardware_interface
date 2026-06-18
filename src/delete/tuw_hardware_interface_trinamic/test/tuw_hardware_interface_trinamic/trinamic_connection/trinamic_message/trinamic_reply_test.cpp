// Copyright 2022 Eugen Kaltenegger

/// Note this may have a problem: https://github.com/google/googletest/issues/930

#include <gtest/gtest.h>

#include "tuw_hardware_interface_trinamic/trinamic_connection/trinamic_message/trinamic_reply.h"

using tuw_hardware_interface::TrinamicReply;

TEST(TrinamicReplyTest, getVelocity0)
{
  TrinamicReply reply(2, 1, 100, 5, 0);
  ASSERT_EQ(reply.toString(), "02 01 64 05 00 00 00 00 6C");
}

TEST(TrinamicReplyTest, setVelocity10)
{
  TrinamicReply reply(2, 1, 100, 5, 10);
  ASSERT_EQ(reply.toString(), "02 01 64 05 00 00 00 0A 76");
}
TEST(TrinamicReplyTest, setVelocity100)
{
  TrinamicReply reply(2, 1, 100, 5, 100);
  ASSERT_EQ(reply.toString(), "02 01 64 05 00 00 00 64 D0");
}

TEST(TrinamicReplyTest, setVelocity1000)
{
  TrinamicReply reply(2, 1, 100, 5, 1000);
  ASSERT_EQ(reply.toString(), "02 01 64 05 00 00 03 E8 57");
}

TEST(TrinamicReplyTest, getVelocity)
{
  TrinamicReply reply(2, 1, 100, 6, 0);
  ASSERT_EQ(reply.toString(), "02 01 64 06 00 00 00 00 6D");
}

