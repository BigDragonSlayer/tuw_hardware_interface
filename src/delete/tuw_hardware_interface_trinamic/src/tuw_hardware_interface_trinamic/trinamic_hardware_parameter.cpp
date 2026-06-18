// Copyright 2022 Eugen Kaltenegger

#include "tuw_hardware_interface_trinamic/trinamic_hardware_parameter.h"

using tuw_hardware_interface::TrinamicHardwareParameter;
using tuw_hardware_interface::TrinamicHardwareParameterDescription;

TrinamicHardwareParameter::TrinamicHardwareParameter(TrinamicHardwareParameterDescription hardware_parameter_description)
{
  this->identifier_ = hardware_parameter_description.getIdentifier();
  this->description_ = hardware_parameter_description.getDescription();
  this->parameter_ = hardware_parameter_description.getParameter();
  this->enum_ = hardware_parameter_description.getEnum();
  this->range_ = hardware_parameter_description.getRange();

  if (!this->isValid())
    throw std::runtime_error("parameter " + *this->getIdentifier() + " is invalid");

  if (this->isTarget())
    this->type_ = Type::TARGET;
  else if (this->isActual())
    this->type_ = Type::ACTUAL;
  else if (this->isEnum())
    this->type_ = Type::ENUM;
  else if (this->isRange())
    this->type_ = Type::RANGE;

  if ((this->type_ == Type::ENUM || this->type_ == Type::RANGE) && this->description_ == nullptr)
    this->description_ = std::make_shared<std::string>("no description provided");
}

std::shared_ptr<int> TrinamicHardwareParameter::getParameter()
{
  return this->parameter_;
}

bool tuw_hardware_interface::TrinamicHardwareParameter::isValid()
{
  return this->isTarget() ^ this->isActual() ^ this->isEnum() ^ this->isRange();
}

bool tuw_hardware_interface::TrinamicHardwareParameter::isTarget()
{
  return this->getIdentifier() &&
         this->getParameter() &&
         this->getRange() &&
         !this->getDescription() &&
         !this->getEnum();
}

bool tuw_hardware_interface::TrinamicHardwareParameter::isActual()
{
  return this->getIdentifier() &&
         this->getParameter() &&
         !this->getDescription() &&
         !this->getRange() &&
         !this->getEnum();
}

bool tuw_hardware_interface::TrinamicHardwareParameter::isRange()
{
  return this->getIdentifier() &&
         this->getDescription() &&
         this->getParameter() &&
         this->getRange() &&
         !this->getEnum();
}

bool tuw_hardware_interface::TrinamicHardwareParameter::isEnum()
{
  return this->getIdentifier() &&
         this->getDescription() &&
         this->getParameter() &&
         this->getEnum() &&
         !this->getRange();
}
