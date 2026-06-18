// Copyright 2022 Eugen Kaltenegger

#ifndef TUW_HARDWARE_INTERFACE_DYNAMIXEL_DYNAMIXEL_CONNECTION_H
#define TUW_HARDWARE_INTERFACE_DYNAMIXEL_DYNAMIXEL_CONNECTION_H

#include <tuw_hardware_interface_template/generic_connection.h>

#include <map>
#include <memory>
#include <string>
#include <dynamixel_sdk/dynamixel_sdk.h>

#define PROTOCOL "2.0"

using dynamixel::PacketHandler;
using dynamixel::PortHandler;

namespace tuw_hardware_interface
{
class DynamixelConnectionDescription;

class DynamixelConnection : public GenericConnection
{
public:
  // singleton function
  static std::shared_ptr<DynamixelConnection> getConnection
          (const std::shared_ptr<DynamixelConnectionDescription>& connection_description);
  // instance functions
  explicit DynamixelConnection(std::shared_ptr<DynamixelConnectionDescription> connection_description);
  ~DynamixelConnection();

  bool connect() override;
  bool disconnect() override;
  void write(int id, GenericHardwareParameter hardware_parameter, int data) override;
  int read(int id, GenericHardwareParameter hardware_parameter) override;
  void read(int id, std::vector<std::pair<GenericHardwareParameter, int*>> parameter_data_pairs) override;
private:
  static std::unique_ptr<std::map<std::string, std::shared_ptr<DynamixelConnection>>> connection_table_;
  std::shared_ptr<DynamixelConnectionDescription> connection_description_;
  std::unique_ptr<dynamixel::PortHandler> port_handler_;
  std::unique_ptr<dynamixel::PacketHandler> packet_handler_;
};
}  // namespace tuw_hardware_interface

#endif  // TUW_HARDWARE_INTERFACE_DYNAMIXEL_DYNAMIXEL_CONNECTION_H
