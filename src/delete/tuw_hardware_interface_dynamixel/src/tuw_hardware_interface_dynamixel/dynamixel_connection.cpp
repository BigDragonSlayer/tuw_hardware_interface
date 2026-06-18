// Copyright 2022 Eugen Kaltenegger

#include <tuw_hardware_interface_dynamixel/description/dynamixel_connection_description.h>
#include <tuw_hardware_interface_dynamixel/dynamixel_connection.h>

#include <tuw_hardware_interface_template/generic_hardware_parameter.h>
#include <tuw_hardware_interface_template/generic_connection.h>

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <ros/ros.h>
#include "tuw_hardware_interface_template/generic_setup_prefix.h"

using tuw_hardware_interface::DynamixelConnection;

std::unique_ptr<std::map<std::string, std::shared_ptr<DynamixelConnection>>> DynamixelConnection::connection_table_;

std::shared_ptr<DynamixelConnection> DynamixelConnection::getConnection
        (const std::shared_ptr<DynamixelConnectionDescription>& connection_description)
{
  std::string connection_hash = connection_description->getHash();

  if (DynamixelConnection::connection_table_ == nullptr)
  {
    DynamixelConnection::mutex_.lock();

    if (DynamixelConnection::connection_table_ == nullptr)

      DynamixelConnection::connection_table_ =
              std::make_unique<std::map<std::string, std::shared_ptr<DynamixelConnection>>>();

    DynamixelConnection::mutex_.unlock();
  }

  if (DynamixelConnection::connection_table_->find(connection_hash) == DynamixelConnection::connection_table_->end())
  {
    std::shared_ptr<DynamixelConnection> connection = std::make_shared<DynamixelConnection>(connection_description);
    DynamixelConnection::connection_table_->insert({connection_hash, connection});
  }

  return DynamixelConnection::connection_table_->at(connection_hash);
}

DynamixelConnection::DynamixelConnection(std::shared_ptr<DynamixelConnectionDescription> connection_description)
        : GenericConnection(connection_description), connection_description_(std::move(connection_description))
{
  DynamixelConnection::connect();
}

DynamixelConnection::~DynamixelConnection()
{
  DynamixelConnection::disconnect();
}

bool DynamixelConnection::connect()
{
  if (PROTOCOL != this->connection_description_->getProtocol())
    throw std::runtime_error("This package supports protocol 2.0 only - requested protocol: " +
                             this->connection_description_->getProtocol());

  this->port_handler_ = std::unique_ptr<PortHandler>(
          PortHandler::getPortHandler(this->connection_description_->getPort().c_str()));
  this->packet_handler_ = std::unique_ptr<PacketHandler>(PacketHandler::getPacketHandler(std::stof(PROTOCOL)));

  if (!this->port_handler_->openPort())
    throw std::runtime_error("connection error - error opening port: " + this->connection_description_->getPort());
  if (!this->port_handler_->setBaudRate(this->connection_description_->getBaudrate()))
    throw std::runtime_error(
            "connection error - error setting baud: " + std::to_string(this->connection_description_->getBaudrate()));

  return true;
}

bool DynamixelConnection::disconnect()
{
  if (this->port_handler_ != nullptr)
  {
    this->port_handler_->closePort();
  }
  return true;
}

void DynamixelConnection::write(int id, GenericHardwareParameter hardware_parameter, int data)
{
  ros::Time start = ros::Time::now();
  dynamixel::PortHandler* port_handler_pointer = this->port_handler_.get();
  auto address = static_cast<uint8_t>(*hardware_parameter.getAddress());
  auto length = static_cast<uint16_t>(*hardware_parameter.getLength());
  int communication_result = COMM_NOT_AVAILABLE;
  unsigned char error = 0;

  uint8_t data_1_byte;
  uint16_t data_2_byte;
  uint32_t data_4_byte;

  switch (length)
  {
    case 1:
      data_1_byte = static_cast<uint8_t>(data);
      this->connection_mutex_.lock();
      communication_result = this->packet_handler_->write1ByteTxRx
              (port_handler_pointer, id, address, data_1_byte, &error);
      this->connection_mutex_.unlock();
      break;
    case 2:
      data_2_byte = static_cast<uint16_t>(data);
      this->connection_mutex_.lock();
      communication_result = this->packet_handler_->write2ByteTxRx
              (port_handler_pointer, id, address, data_2_byte, &error);
      this->connection_mutex_.unlock();
      break;
    case 4:
      data_4_byte = static_cast<uint32_t>(data);
      this->connection_mutex_.lock();
      communication_result = this->packet_handler_->write4ByteTxRx
              (port_handler_pointer, id, address, data_4_byte, &error);
      this->connection_mutex_.unlock();
      break;
    default:
      std::string error_message("communication error - message size is invalid: " + std::to_string(length));
      throw std::runtime_error(error_message);
  }

  if (communication_result != COMM_SUCCESS)
  {
    std::string error_message(
            "communication error (joint ID: " + std::to_string(id) + ") - " +
            "writing parameter " + *hardware_parameter.getIdentifier());
    ROS_WARN("%s", error_message.c_str());
    throw std::runtime_error(error_message);
  }

  ros::Time end = ros::Time::now();
  ros::Duration duration = ros::Duration(end - start);
  ROS_DEBUG("dynamixel write was: %ld", duration.toNSec());
}

int DynamixelConnection::read(int id, GenericHardwareParameter hardware_parameter)
{
  ros::Time start = ros::Time::now();

  dynamixel::PortHandler* port_handler_pointer = this->port_handler_.get();
  auto address = static_cast<uint8_t>(*hardware_parameter.getAddress());
  auto length = static_cast<uint16_t>(*hardware_parameter.getLength());
  int communication_result = COMM_NOT_AVAILABLE;
  int data;

  uint8_t data_1_byte = 0;
  uint16_t data_2_byte = 0;
  uint32_t data_4_byte = 0;

  switch (length)
  {
    case 1:
      this->connection_mutex_.lock();
      communication_result = this->packet_handler_->read1ByteTxRx(port_handler_pointer, id, address, &data_1_byte);
      this->connection_mutex_.unlock();
      data = static_cast<int>(data_1_byte);
      break;
    case 2:
      this->connection_mutex_.lock();
      communication_result = this->packet_handler_->read2ByteTxRx(port_handler_pointer, id, address, &data_2_byte);
      this->connection_mutex_.unlock();
      data = static_cast<int>(data_2_byte);
      break;
    case 4:
      this->connection_mutex_.lock();
      communication_result = this->packet_handler_->read4ByteTxRx(port_handler_pointer, id, address, &data_4_byte);
      this->connection_mutex_.unlock();
      data = static_cast<int>(data_4_byte);
      break;
    default:
      std::string error_message("communication error - message length is invalid: " + std::to_string(length));
      throw std::runtime_error(error_message);
  }

  if (communication_result != COMM_SUCCESS)
  {
    std::string error_message(
            "communication error (joint ID: " + std::to_string(id) + ") - " +
            "reading parameter " + *hardware_parameter.getIdentifier());
    throw std::runtime_error(error_message);
  }

  ros::Time end = ros::Time::now();
  ros::Duration duration = ros::Duration(end - start);
  ROS_DEBUG("dynamixel read was: %ld", duration.toNSec());

  return data;
}

void DynamixelConnection::read(int id, std::vector<std::pair<GenericHardwareParameter, int*>> parameter_data_pairs)
{
  ros::Time start = ros::Time::now();

  dynamixel::GroupBulkRead bulk_read(this->port_handler_.get(), this->packet_handler_.get());

  GenericHardwareParameter min = parameter_data_pairs[0].first;
  GenericHardwareParameter max = parameter_data_pairs[0].first;

  for (auto parameter_data_pair : parameter_data_pairs)
  {
    int address = *parameter_data_pair.first.getAddress();
    if (address < *min.getAddress())
      min = parameter_data_pair.first;
    if (address > *max.getAddress())
      max = parameter_data_pair.first;
  }

  bulk_read.addParam(id, *min.getAddress(), *max.getAddress() - *min.getAddress() + *max.getLength());

  this->connection_mutex_.lock();
  bulk_read.txRxPacket();
  this->connection_mutex_.unlock();

  for (auto parameter_data_pair : parameter_data_pairs)
  {
    auto identifier = *parameter_data_pair.first.getIdentifier();
    int address = *parameter_data_pair.first.getAddress();
    int length = *parameter_data_pair.first.getLength();

    auto data_pointer = parameter_data_pair.second;

    if (bulk_read.isAvailable(id, address, length))
    {
      *data_pointer = static_cast<int>(bulk_read.getData(id, address, length));
    }
    else
    {
      ROS_WARN("[%s] ERROR reading %s from device with ID %d", PREFIX, identifier.LOG, id);
    }
  }

  ros::Time end = ros::Time::now();
  ros::Duration duration = ros::Duration(end - start);
  ROS_DEBUG("dynamixel bulk read was: %ld", duration.toNSec());
}
