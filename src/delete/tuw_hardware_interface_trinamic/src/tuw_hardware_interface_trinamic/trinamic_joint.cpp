// Copyright 2023 Eugen Kaltenegger

#include "tuw_hardware_interface_trinamic/trinamic_joint.h"
#include "tuw_hardware_interface_template/description/generic_joint_description.h"
#include "tuw_hardware_interface_template/generic_setup_prefix.h"
#include "tuw_hardware_interface_trinamic/trinamic_hardware.h"

using tuw_hardware_interface::TrinamicJoint;

void TrinamicJoint::setTrinamicConnection(std::shared_ptr<TMCM1640Connection> connection)
{
  this->connection_ = connection;
}

void TrinamicJoint::writeTrinamic(TrinamicHardwareParameter hardware_parameter, int data)
{
  this->connection_->writeTrinamic(this->id_, hardware_parameter, data);
}

int TrinamicJoint::readTrinamic(TrinamicHardwareParameter hardware_parameter)
{
  return this->connection_->readTrinamic(this->id_, hardware_parameter);
}

tuw_hardware_interface::TrinamicJoint::TrinamicJoint(GenericJointDescription joint_description)
        : GenericJoint(joint_description)
{

}

//void tuw_hardware_interface::TrinamicJoint::writeTarget(double target, GenericHardware::Mode mode,
//                                                        const std::string& mode_name)
//{
//  if (this->hardware_->supportsTargetMode(mode))
//  {
//    int hardware_target = this->hardware_->convertToHardwareResolution(target, mode);
//    this->connection_->writeTrinamic(this->id_, this->hardware_->getTargetTrinamicParameterForMode(mode), hardware_target);
//  }
//  else
//  {
//    ROS_WARN("[%s] %s is not supporting target mode %s", PREFIX, this->hardware_->getName().LOG, mode_name.LOG);
//    throw std::runtime_error("unsupported target mode requested");
//  }
//}

void tuw_hardware_interface::TrinamicJoint::writeTarget(double target, GenericHardware::Mode mode,
                                                        const std::string& mode_name)
{
  if (this->hardware_->supportsTargetMode(mode))
  {
    if (mode == GenericHardware::Mode::POSITION)
    {
      int hardware_target = this->hardware_->convertToHardwareResolution(target, mode);
      this->connection_->writeTrinamic(this->id_, this->hardware_->getTargetTrinamicParameterForMode(mode), hardware_target);
    }
    if (mode == GenericHardware::Mode::VELOCITY)
    {
      int hardware_target;
      if (this->diameter_ > 0)
      {
        hardware_target = this->hardware_->convertToHardwareResolution(this->MPStoRadPS(target), mode);
      }
      else
      {
        hardware_target = this->hardware_->convertToHardwareResolution(target, mode);
      }
      this->connection_->writeTrinamic(this->id_, this->hardware_->getTargetTrinamicParameterForMode(mode), hardware_target);
    }
    if (mode == GenericHardware::Mode::EFFORT)
    {
      int hardware_target = this->hardware_->convertToHardwareResolution(target, mode);
      this->connection_->writeTrinamic(this->id_, this->hardware_->getTargetTrinamicParameterForMode(mode), hardware_target);
    }
  }
  else
  {
    ROS_WARN("[%s] %s is not supporting target mode %s", PREFIX, this->hardware_->getName().LOG, mode_name.LOG);
    throw std::runtime_error("unsupported target mode requested");
  }
}

double tuw_hardware_interface::TrinamicJoint::readActual(GenericHardware::Mode mode, const std::string& mode_name)
{
  if (this->hardware_->supportsActualMode(mode))
  {
    int hardware_actual = this->connection_->readTrinamic(this->id_, this->hardware_->getActualTrinamicParameterForMode(mode));
    return this->hardware_->convertFromHardwareResolution(hardware_actual, mode);
  }
  else
  {
    ROS_WARN("[%s] %s is not supporting target mode %s", PREFIX, this->hardware_->getName().LOG, mode_name.LOG);
    throw std::runtime_error("unsupported actual mode requested");
  }
}

void TrinamicJoint::setTrinamicHardware(std::shared_ptr<TrinamicHardware> hardware)
{
  GenericJoint::hardware_ = hardware;
  this->hardware_ = hardware;
}

void TrinamicJoint::write(const ros::Duration& period)
{
  GenericJoint::write(period);
}

//void TrinamicJoint::read(const ros::Duration& period)
//{
//  auto map = std::map<GenericHardware::Mode, std::shared_ptr<int>>();
//  auto actual = std::vector<std::pair<TrinamicHardwareParameter, int*>>();
//  for (GenericHardware::Mode mode : this->hardware_->getSupportedActualModes())
//  {
//    map.insert({mode, std::make_shared<int>()});
//    actual.push_back({this->hardware_->getActualTrinamicParameterForMode(mode), map.at(mode).get()});
//  }
//
//  this->connection_->readTrinamic(this->id_, actual);
//
//  for (GenericHardware::Mode mode : this->hardware_->getSupportedActualModes())
//  {
//    if (mode == GenericHardware::Mode::POSITION)
//      this->actual_position_ = this->hardware_->convertFromHardwareResolution(*map.at(mode), mode);
//    if (mode == GenericHardware::Mode::VELOCITY)
//      this->actual_velocity_ = this->hardware_->convertFromHardwareResolution(*map.at(mode), mode);
//    if (mode == GenericHardware::Mode::EFFORT)
//      this->actual_effort_ = this->hardware_->convertFromHardwareResolution(*map.at(mode), mode);
//  }
//}

void TrinamicJoint::read(const ros::Duration& period)
{
  auto map = std::map<GenericHardware::Mode, std::shared_ptr<int>>();
  auto actual = std::vector<std::pair<TrinamicHardwareParameter, int*>>();
  for (GenericHardware::Mode mode : this->hardware_->getSupportedActualModes())
  {
    map.insert({mode, std::make_shared<int>()});
    actual.emplace_back(this->hardware_->getActualTrinamicParameterForMode(mode), map.at(mode).get());
  }

  this->connection_->readTrinamic(this->id_, actual);

  for (GenericHardware::Mode mode : this->hardware_->getSupportedActualModes())
  {
    if (mode == GenericHardware::Mode::POSITION)
      this->actual_position_ = this->hardware_->convertFromHardwareResolution(*map.at(mode), mode);
    if (mode == GenericHardware::Mode::VELOCITY)
    {
      if (this->diameter_ > 0)
        this->actual_velocity_ = this->RadPStoMPS(this->hardware_->convertFromHardwareResolution(*map.at(mode), mode));
      else
        this->actual_velocity_ = this->hardware_->convertFromHardwareResolution(*map.at(mode), mode);
    }
    if (mode == GenericHardware::Mode::EFFORT)
      this->actual_effort_ = this->hardware_->convertFromHardwareResolution(*map.at(mode), mode);
  }
}
