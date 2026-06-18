// Copyright 2022 Eugen Kaltenegger

#include <tuw_hardware_interface_trinamic/trinamic_connection/trinamic_message/trinamic_message.h>

using tuw_hardware_interface::TrinamicMessage;


tuw_hardware_interface::TrinamicMessage::TrinamicMessage(unsigned char byte1,
                                                         unsigned char byte2,
                                                         unsigned char byte3,
                                                         unsigned char byte4,
                                                         int value)
{
  this->setByte1(byte1);
  this->setByte2(byte2);
  this->setByte3(byte3);
  this->setByte4(byte4);
  this->setValue(value);
}

unsigned char* TrinamicMessage::getBufferPointer()
{
  return this->buffer_;
}

size_t TrinamicMessage::getBufferSize()
{
  return sizeof(this->buffer_);
}

void TrinamicMessage::setByte1(unsigned char byte)
{
  *this->module_address_ = byte;
}

unsigned char TrinamicMessage::getByte1()
{
  return *this->module_address_;
}

void TrinamicMessage::setByte2(unsigned char byte)
{
  *this->command_number_ = byte;
}

unsigned char TrinamicMessage::getByte2()
{
  return *this->command_number_;
}

void TrinamicMessage::setByte3(unsigned char byte)
{
  *this->command_type_ = byte;
}

unsigned char tuw_hardware_interface::TrinamicMessage::getByte3()
{
  return *this->command_type_;
}

void TrinamicMessage::setByte4(unsigned char byte)
{
  *this->motor_number_ = byte;
}

unsigned char TrinamicMessage::getByte4()
{
  return *this->motor_number_;
}

void TrinamicMessage::setValue(int value)
{
  this->buffer_[4] = (value >> 24) & 0xFF;
  this->buffer_[5] = (value >> 16) & 0xFF;
  this->buffer_[6] = (value >> 8) & 0xFF;
  this->buffer_[7] = value & 0xFF;
}

int TrinamicMessage::getValue()
{
  return (this->buffer_[4] << 24 | this->buffer_[5] << 16 | this->buffer_[6] << 8 | this->buffer_[7]);
}

void TrinamicMessage::setChecksum(unsigned char checksum)
{
  *this->checksum_ = checksum;
}

unsigned char TrinamicMessage::getChecksum()
{
  return *this->checksum_;
}

unsigned char TrinamicMessage::calculateChecksum()
{
  unsigned char checksum = 0;
  for (unsigned char i = 0; i < 8; i++)
  {
    checksum += this->buffer_[i];
  }
  return checksum;
}

std::string TrinamicMessage::toString()
{
  std::string string;
  for (int i = 0; i < 9; i++)
  {
    // convert to HEX
    char hex_char[3];
    snprintf(hex_char, sizeof(hex_char), "%02X", this->buffer_[i]);
    // add HEX to string
    string += std::string(hex_char);
    // add whitespace to string (if another HEX follows)
    if (i != 8)
    {
      string += std::string(" ");
    }
  }
  return string;
}
