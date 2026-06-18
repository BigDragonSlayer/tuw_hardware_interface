// Copyright 2021 Eugen Kaltenegger

#include <tuw_hardware_interface_template/description/generic_connection_description.h>

#include <string>

using tuw_hardware_interface::GenericConnectionDescription;

GenericConnectionDescription::GenericConnectionDescription(YAML::Node yaml)
{
  this->hash_ = yaml["port"].as<std::string>() + "_" + yaml["baudrate"].as<std::string>();
  this->port_ = yaml["port"].as<std::string>();
  this->baudrate_ = yaml["baudrate"].as<int>();
}

std::string GenericConnectionDescription::getHash()
{
  return this->hash_;
}

std::string GenericConnectionDescription::getPort()
{
  return this->port_;
}

int GenericConnectionDescription::getBaudrate() const
{
  return this->baudrate_;
}
