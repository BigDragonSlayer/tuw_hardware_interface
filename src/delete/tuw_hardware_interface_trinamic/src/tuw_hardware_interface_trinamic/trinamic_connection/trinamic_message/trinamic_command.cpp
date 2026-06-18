// Copyright 2022 Eugen Kaltenegger

#include <tuw_hardware_interface_trinamic/trinamic_connection/trinamic_message/trinamic_command.h>

using tuw_hardware_interface::TrinamicCommand;

TrinamicCommand::TrinamicCommand(unsigned char module_address,
                                 unsigned char command,
                                 unsigned char type,
                                 unsigned char id,
                                 int value)
{
  this->setModuleAddress(module_address);
  this->setCommand(command);
  this->setType(type);
  this->setId(id);
  this->setValue(value);
  this->setChecksum(this->calculateChecksum());
}


void TrinamicCommand::setModuleAddress(unsigned char module_address)
{
  this->setByte1(module_address);
}

void TrinamicCommand::setCommand(unsigned char command)
{
  this->setByte2(command);
}

void TrinamicCommand::setType(unsigned char type)
{
  this->setByte3(type);
}

void TrinamicCommand::setId(unsigned char id)
{
  this->setByte4(id);
}
