// Copyright 2022 Eugen Kaltenegger

#ifndef DIP_WS_TRINAMIC_COMMAND_H
#define DIP_WS_TRINAMIC_COMMAND_H

#include <tuw_hardware_interface_trinamic/trinamic_connection/trinamic_message/trinamic_message.h>

namespace tuw_hardware_interface
{
class TrinamicCommand : public TrinamicMessage
{
public:
  TrinamicCommand(unsigned char module_address,
                  unsigned char command,
                  unsigned char type,
                  unsigned char id,
                  int value);
  TrinamicCommand() = default;
  ~TrinamicCommand() = default;
protected:
  void setModuleAddress(unsigned char module_address);
  void setCommand(unsigned char command);
  void setType(unsigned char type);
  void setId(unsigned char id);
};
}  // namespace tuw_hardware_interface


#endif //DIP_WS_TRINAMIC_COMMAND_H
