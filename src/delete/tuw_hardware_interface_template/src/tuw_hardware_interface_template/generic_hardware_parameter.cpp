// Copyright 2022 Eugen Kaltenegger

#include <tuw_hardware_interface_template/generic_hardware_parameter.h>

#include <map>
#include <memory>
#include <string>

using tuw_hardware_interface::GenericHardwareParameter;
using tuw_hardware_interface::GenericHardwareParameterDescription;

GenericHardwareParameter::GenericHardwareParameter(GenericHardwareParameterDescription hardware_parameter_description)
{
  this->identifier_ = hardware_parameter_description.getIdentifier();
  this->description_ = hardware_parameter_description.getDescription();
  this->address_ = hardware_parameter_description.getAddress();
  this->length_ = hardware_parameter_description.getLength();
  this->enum_ = hardware_parameter_description.getEnum();
  this->range_ = hardware_parameter_description.getRange();

  if (!this->isValid())
    throw std::runtime_error("parameter " + *this->getIdentifier() + " is invalid");

  if (this->isTarget())
    this->type_ = Type::TARGET;
  else if (this->isActual())
    this->type_ = Type::ACTUAL;
  else if (this->isEnum()  )
    this->type_ = Type::ENUM;
  else if (this->isRange() )
    this->type_ = Type::RANGE;

  if ((this->type_ == Type::ENUM || this->type_ == Type::RANGE) && this->description_ == nullptr)
    this->description_ = std::make_shared<std::string>("no description provided");
}

GenericHardwareParameter::Type GenericHardwareParameter::getType()
{
  return this->type_;
}

bool GenericHardwareParameter::isValid()
{
  return this->isTarget() ^ this->isActual() ^ this->isEnum() ^ this->isRange();
}

bool GenericHardwareParameter::isTarget()
{
  return this->getIdentifier() &&
         this->getAddress() &&
         this->getLength() &&
         this->getRange() &&
         !this->getDescription() &&
         !this->getEnum();
}

bool GenericHardwareParameter::isActual()
{
  return this->getIdentifier() &&
         this->getAddress() &&
         this->getLength() &&
         !this->getDescription() &&
         !this->getRange() &&
         !this->getEnum();
}

bool GenericHardwareParameter::isEnum()
{
  return this->getIdentifier() &&
         this->getDescription() &&
         this->getAddress() &&
         this->getLength() &&
         this->getEnum() &&
         !this->getRange();
}

bool GenericHardwareParameter::isRange()
{
  return this->getIdentifier() &&
         this->getDescription() &&
         this->getAddress() &&
         this->getLength() &&
         this->getRange() &&
         !this->getEnum();
}

std::shared_ptr<std::string> GenericHardwareParameter::getIdentifier()
{
  return this->identifier_;
}

std::shared_ptr<std::string> GenericHardwareParameter::getDescription()
{
  return this->description_;
}

std::shared_ptr<int> GenericHardwareParameter::getAddress()
{
  return this->address_;
}

std::shared_ptr<int> GenericHardwareParameter::getLength()
{
  return this->length_;
}

std::shared_ptr<std::map<std::string, int>> GenericHardwareParameter::getEnum()
{
  return this->enum_;
}

std::shared_ptr<std::map<std::string, int>> GenericHardwareParameter::getRange()
{
  return this->range_;
}

bool GenericHardwareParameter::operator==(const GenericHardwareParameter &other) const
{
  return this->identifier_ == other.identifier_ && this->address_ == other.address_ && this->length_ == other.length_;
}
