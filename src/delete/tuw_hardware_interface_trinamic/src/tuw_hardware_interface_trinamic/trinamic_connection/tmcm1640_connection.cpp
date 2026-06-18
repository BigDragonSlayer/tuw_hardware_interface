// Copyright 2022 Eugen Kaltenegger

#include "tuw_hardware_interface_trinamic/trinamic_connection/tmcm1640_connection.h"

#include "tuw_hardware_interface_template/description/generic_connection_description.h"
#include "tuw_hardware_interface_template/generic_hardware_parameter.h"
#include "tuw_hardware_interface_template/generic_setup_prefix.h"

#include <tuw_hardware_interface_trinamic/trinamic_connection/trinamic_message/trinamic_command.h>
#include <tuw_hardware_interface_trinamic/trinamic_connection/trinamic_message/trinamic_reply.h>
#include <boost/format.hpp>
#include <ros/ros.h>

using tuw_hardware_interface::TMCM1640Connection;
using tuw_hardware_interface::TrinamicCommand;
using tuw_hardware_interface::TrinamicReply;
using tuw_hardware_interface::GenericHardwareParameter;
using tuw_hardware_interface::GenericSetupPrefix;

std::unique_ptr<std::map<std::string, std::shared_ptr<TMCM1640Connection>>> TMCM1640Connection::connection_table_;

std::shared_ptr<TMCM1640Connection> TMCM1640Connection::getConnection(const std::shared_ptr<GenericConnectionDescription>& connection_description)
{
  std::string connection_hash = connection_description->getHash();

  if (TMCM1640Connection::connection_table_ == nullptr)
  {
    TMCM1640Connection::mutex_.lock();

    if (TMCM1640Connection::connection_table_ == nullptr)

      TMCM1640Connection::connection_table_ =
              std::make_unique<std::map<std::string, std::shared_ptr<TMCM1640Connection>>>();

    TMCM1640Connection::mutex_.unlock();
  }

  if (TMCM1640Connection::connection_table_->find(connection_hash) == TMCM1640Connection::connection_table_->end())
  {
    std::shared_ptr<TMCM1640Connection> connection = std::make_shared<TMCM1640Connection>(connection_description);
    TMCM1640Connection::connection_table_->insert({connection_hash, connection});
  }

  return TMCM1640Connection::connection_table_->at(connection_hash);
}

TMCM1640Connection::TMCM1640Connection(std::shared_ptr<GenericConnectionDescription> connection_description)
{
  this->connection_description_ = connection_description;
  this->connect();
}

TMCM1640Connection::~TMCM1640Connection()
{
  this->disconnect();
}

bool TMCM1640Connection::connect()
{
//   this code is from: https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
//   open serial port file
  this->serial_port_ = open(this->connection_description_->getPort().c_str(), O_RDWR);

  // read settings and check for errors
  if (tcgetattr(this->serial_port_, &this->tty_) != 0)
  {
    std::error_code error_code(errno, std::generic_category());
    std::string error_message =
            (boost::format("open port: error %i from tcgetattr: %s") % errno % strerror(errno)).str();
    throw std::system_error(error_code, error_message.c_str());
  }

  // clear parity bit
  this->tty_.c_cflag &= ~PARENB;
  // clear stop field
  this->tty_.c_cflag &= ~CSTOPB;
  // clear size bit
  this->tty_.c_cflag &= ~CSIZE;
  // set size bit
  this->tty_.c_cflag |= CS8;
  // disable RTS/CTS hardware flow control
  this->tty_.c_cflag &= CRTSCTS;
  // turn on read (and ignore control lines)
  this->tty_.c_cflag |= CREAD | CLOCAL;

  this->tty_.c_lflag &= ~ICANON;
  // disable echo
  this->tty_.c_lflag &= ~ECHO;
  // disable erasure
  this->tty_.c_lflag &= ~ECHOE;
  // disable new-line echo
  this->tty_.c_lflag &= ~ECHONL;
  // disable interpretation of INTR, QUIT ans SUSP
  this->tty_.c_lflag &= ~ISIG;
  // disable s/w flow control
  this->tty_.c_iflag &= ~(IXON | IXOFF | IXANY);
  // disable special handling of received bytes
  this->tty_.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

  // prevent special interpretation of output bytes
  this->tty_.c_oflag &= ~OPOST;
  // prevent conversion of new-line to carriage return
  this->tty_.c_oflag &= ~ONLCR;

  // set wait for 9 bytes
  this->tty_.c_cc[VTIME] = 0;
  this->tty_.c_cc[VMIN] = 9;

  // set baud rate for input
  cfsetispeed(&tty_, B9600);
  // set baud rate for output
  cfsetospeed(&tty_, B9600);

  // write settings and check for errors
  if (tcsetattr(this->serial_port_, TCSANOW, &this->tty_) != 0)
  {
    std::error_code error_code(errno, std::generic_category());
    std::string error_message =
            (boost::format("open port: error %i from tcsetattr: %s") % errno % strerror(errno)).str();
    throw std::system_error(error_code, error_message.c_str());
  }
  return true;
}

bool TMCM1640Connection::disconnect()
{
  close(this->serial_port_);
  return true;
}

void TMCM1640Connection::write(int id, GenericHardwareParameter hardware_parameter, int data)
{
  ROS_INFO("parent_write");
}

int TMCM1640Connection::read(int id, GenericHardwareParameter hardware_parameter)
{
  ROS_INFO("parent_read");
  return 0;
}

void TMCM1640Connection::writeTrinamic(int id, TrinamicHardwareParameter hardware_parameter, int data)
{
  ros::Time start = ros::Time::now();
  unsigned char module_address = 1;
  unsigned char command_number = SET_AXIS_PARAMETER;
  auto type_number = static_cast<unsigned char>(*hardware_parameter.getParameter());
  auto id_number = static_cast<unsigned char>(id);

  TrinamicCommand command(module_address, command_number, type_number, id_number, data);
  TrinamicReply reply = this->communicate(command);

  ros::Time end = ros::Time::now();
  ros::Duration duration = ros::Duration(end - start);
  ROS_DEBUG("trinamic write was: %ld", duration.toNSec());
}

int TMCM1640Connection::readTrinamic(int id, TrinamicHardwareParameter hardware_parameter)
{
  ros::Time start = ros::Time::now();
  unsigned char module_address = 1;
  unsigned char command_number = GET_AXIS_PARAMETER;
  auto type_number = static_cast<unsigned char>(*hardware_parameter.getParameter());
  auto id_number = static_cast<unsigned char>(id);

  TrinamicCommand command(module_address, command_number, type_number, id_number, 0);
  TrinamicReply reply = this->communicate(command);
  return reply.getValue();

  ros::Time end = ros::Time::now();
  ros::Duration duration = ros::Duration(end - start);
  ROS_DEBUG("trinamic read was: %ld", duration.toNSec());
}

TrinamicReply TMCM1640Connection::communicate(TrinamicCommand command)
{
  TrinamicReply reply;

  this->connection_mutex_.lock();
  // write data
  ::write(this->serial_port_, command.getBufferPointer(), command.getBufferSize());
  ROS_DEBUG("[%s] sending command to port %s: %s",
            PREFIX, this->connection_description_->getPort().LOG, command.toString().LOG);

  // read data
  ::read(this->serial_port_, reply.getBufferPointer(), reply.getBufferSize());
  ROS_DEBUG("[%s] receiving reply from port %s: %s",
            PREFIX, this->connection_description_->getPort().LOG, reply.toString().LOG);
  this->connection_mutex_.unlock();

  if (reply.getChecksum() != reply.calculateChecksum())
  {
    std::error_code error_code(errno, std::generic_category());
    std::string error_message("communication error - checksum not correct");
    throw std::system_error(error_code, error_message.c_str());
  }
  if (reply.getStatus() == SUCCESS)
  {
    return reply;
  }
  else
  {
    std::string error_message_substring;
    switch (reply.getStatus())
    {
      case WRONG_CHECKSUM:
        error_message_substring = "wrong checksum";
        break;
      case INVALID_COMMAND:
        error_message_substring = "invalid command";
        break;
      case WRONG_TYPE:
        error_message_substring = "wrong type";
        break;
      case INVALID_VALUE:
        error_message_substring = "invalid value";
        break;
      case CONFIGURATION_LOCKED:
        error_message_substring = "configuration locked";
        break;
      case COMMAND_NOT_AVAILABLE:
        error_message_substring = "command not available";
        break;
    }
    std::error_code error_code(errno, std::generic_category());
    std::string error_message = (boost::format("communication error - %s") % error_message_substring).str();
    throw std::system_error(error_code, error_message.c_str());
  }
}

void TMCM1640Connection::readTrinamic(int id, std::vector<std::pair<TrinamicHardwareParameter, int*>> parameter_data_pairs)
{
  for (auto parameter_data_pair : parameter_data_pairs)
  {
    auto hardware_parameter = parameter_data_pair.first;
    auto data_pointer = parameter_data_pair.second;
    *data_pointer = this->readTrinamic(id, hardware_parameter);
  }
}
