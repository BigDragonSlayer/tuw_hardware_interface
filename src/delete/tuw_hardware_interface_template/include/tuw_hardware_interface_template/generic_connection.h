// Copyright 2022 Eugen Kaltenegger

#ifndef TUW_HARDWARE_INTERFACE_TEMPLATE_GENERIC_CONNECTION_H
#define TUW_HARDWARE_INTERFACE_TEMPLATE_GENERIC_CONNECTION_H

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <list>
#include "generic_hardware.h"

namespace tuw_hardware_interface
{

class GenericJoint;
class GenericHardwareParameter;
class GenericConnectionDescription;

class GenericConnection
{
public:
  // singleton function
  static std::shared_ptr<GenericConnection> getConnection
          (const std::shared_ptr<GenericConnectionDescription>& connection_description);
  // instance functions
  explicit GenericConnection(std::shared_ptr<GenericConnectionDescription> connection_description);
  GenericConnection();
  ~GenericConnection();
  virtual bool connect() = 0;
  virtual bool disconnect() = 0;
  virtual void write(int id, GenericHardwareParameter hardware_parameter, int data) = 0;
  virtual int read(int id, GenericHardwareParameter hardware_parameter) = 0;
  virtual void read(int id, std::vector<std::pair<GenericHardwareParameter, int*>> parameter_data_pairs);

  protected:
  // singleton variables
  static std::mutex mutex_;
  static std::unique_ptr<std::map<std::string, std::shared_ptr<GenericConnection>>> connection_table_;
  // instance variables
  std::mutex connection_mutex_;
  std::list<GenericJoint*> joints_;
};
}  // namespace tuw_hardware_interface

#endif  // TUW_HARDWARE_INTERFACE_TEMPLATE_GENERIC_CONNECTION_H
