// Copyright 2022 Eugen Kaltenegger

#include <tuw_hardware_interface_template/generic_connection.h>

#include <map>
#include <memory>
#include <string>

#include <tuw_hardware_interface_template/description/generic_connection_description.h>

using tuw_hardware_interface::GenericConnection;
using tuw_hardware_interface::GenericConnectionDescription;

std::mutex GenericConnection::mutex_;
std::unique_ptr<std::map<std::string, std::shared_ptr<GenericConnection>>> GenericConnection::connection_table_;

std::shared_ptr<GenericConnection> GenericConnection::getConnection
        (const std::shared_ptr<GenericConnectionDescription>& connection_description)
{
  std::string connection_hash = connection_description->getHash();

  if (GenericConnection::connection_table_ == nullptr)
  {
    GenericConnection::mutex_.lock();

    if (GenericConnection::connection_table_ == nullptr)

      GenericConnection::connection_table_ =
              std::make_unique<std::map<std::string, std::shared_ptr<GenericConnection>>>();

    GenericConnection::mutex_.unlock();
  }

  if (GenericConnection::connection_table_->find(connection_hash) == GenericConnection::connection_table_->end())
  {
    // in implementations this should create a connection
    std::shared_ptr<GenericConnection> connection = nullptr;
    GenericConnection::connection_table_->insert({connection_hash, connection});
  }

  return GenericConnection::connection_table_->at(connection_hash);
}

GenericConnection::GenericConnection(std::shared_ptr<GenericConnectionDescription> connection_description)
{
  // call connect in implementation
}

GenericConnection::GenericConnection()
{
  // constructor for mocking
}

GenericConnection::~GenericConnection()
{
  // call disconnect in implementation
}

void GenericConnection::read(int id, std::vector<std::pair<GenericHardwareParameter, int*>> parameter_data_pairs)
{
  for (auto parameter_data_pair : parameter_data_pairs)
  {
    auto hardware_parameter = parameter_data_pair.first;
    auto data_pointer = parameter_data_pair.second;
    *data_pointer = this->read(id, hardware_parameter);
  }
}
