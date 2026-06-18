// Copyright 2022 Eugen Kaltenegger

#include <memory>
#include <string>
#include <utility>

#include <tuw_hardware_interface_template/description/generic_joint_description.h>
#include <tuw_hardware_interface_template/generic_connection.h>
#include <tuw_hardware_interface_template/generic_hardware.h>
#include <tuw_hardware_interface_template/generic_joint.h>
#include <tuw_hardware_interface_template/generic_setup_prefix.h>

#include <urdf/model.h>

using tuw_hardware_interface::GenericConnection;
using tuw_hardware_interface::GenericHardware;
using tuw_hardware_interface::GenericHardwareParameter;
using tuw_hardware_interface::GenericJoint;
using tuw_hardware_interface::GenericJointDescription;

GenericJoint::GenericJoint(GenericJointDescription joint_description)
{
  this->name_ = joint_description.getName();
  this->id_ = joint_description.getId();
  this->diameter_ = joint_description.getDiameter();

  this->joint_state_handle_ = std::make_unique<JointStateHandle>
          (this->name_, &this->actual_position_, &this->actual_velocity_, &this->actual_effort_);

  this->joint_position_handle_ = std::make_unique<JointHandle>
          (*this->joint_state_handle_, &this->target_position_);
  this->joint_velocity_handle_ = std::make_unique<JointHandle>
          (*this->joint_state_handle_, &this->target_velocity_);
  this->joint_effort_handle_ = std::make_unique<JointHandle>
          (*this->joint_state_handle_, &this->target_effort_);

  urdf::Model urdf_model;
  urdf_model.initParam("/robot_description");

  const bool has_limits      = getJointLimits(urdf_model.getJoint(this->name_), *this->limits_);
  const bool has_soft_limits = getSoftJointLimits(urdf_model.getJoint(this->name_), *this->soft_limits_);

  if (has_limits && has_soft_limits)
  {
    this->joint_position_soft_limit_handle_ = std::make_unique<PositionJointSoftLimitsHandle>
            (*this->joint_position_handle_, *this->limits_, *this->soft_limits_);
    this->joint_velocity_soft_limit_handle_ = std::make_unique<VelocityJointSoftLimitsHandle>
            (*this->joint_velocity_handle_, *this->limits_, *this->soft_limits_);
    this->joint_effort_soft_limit_handle_ = std::make_unique<EffortJointSoftLimitsHandle>
            (*this->joint_effort_handle_, *this->limits_, *this->soft_limits_);
  }

  if (has_limits && !has_soft_limits)
  {
    this->joint_position_limit_handle_ = std::make_unique<PositionJointSaturationHandle>
            (*this->joint_position_handle_, *this->limits_);
    this->joint_velocity_limit_handle_ = std::make_unique<VelocityJointSaturationHandle>
            (*this->joint_velocity_handle_, *this->limits_);
    this->joint_effort_limit_handle_ = std::make_unique<EffortJointSaturationHandle>
            (*this->joint_effort_handle_, *this->limits_);
  }
}

void GenericJoint::setConnection(std::shared_ptr<GenericConnection> connection)
{
  this->connection_ = std::move(connection);
}

void GenericJoint::setHardware(std::shared_ptr<GenericHardware> hardware)
{
  this->hardware_ = std::move(hardware);
}

void GenericJoint::setConfig(std::shared_ptr<GenericConfig> config)
{
  this->config_ = std::move(config);
}

std::string GenericJoint::getName()
{
  return this->name_;
}

int GenericJoint::getId()
{
  return this->id_;
}

int GenericJoint::getDiameter()
{
  return this->diameter_;
}

void GenericJoint::write(const ros::Duration &period)
{
  if (!this->mode_)
  {
    ROS_DEBUG("[%s] operation mode for joint %s is (yet?) not defined", PREFIX, this->name_.LOG);
    return;
  }

  switch (*this->mode_)
  {
    case GenericHardware::Mode::POSITION:
      if (this->joint_position_soft_limit_handle_ != nullptr)
        this->joint_position_soft_limit_handle_->enforceLimits(period);
      else if (this->joint_position_limit_handle_ != nullptr)
        this->joint_position_limit_handle_->enforceLimits(period);
      this->writeTarget(this->target_position_, GenericHardware::Mode::POSITION, "POSITION");
      break;
    case GenericHardware::Mode::VELOCITY:
      if (this->joint_velocity_soft_limit_handle_ != nullptr)
        this->joint_velocity_soft_limit_handle_->enforceLimits(period);
      else if (this->joint_velocity_limit_handle_ != nullptr)
        this->joint_velocity_limit_handle_->enforceLimits(period);
      this->writeTarget(this->target_velocity_, GenericHardware::Mode::VELOCITY, "VELOCITY");
      break;
    case GenericHardware::Mode::EFFORT:
      if (this->joint_effort_soft_limit_handle_ != nullptr)
        this->joint_effort_soft_limit_handle_->enforceLimits(period);
      else if (this->joint_effort_limit_handle_ != nullptr)
        this->joint_effort_limit_handle_->enforceLimits(period);
      this->writeTarget(this->target_effort_, GenericHardware::Mode::EFFORT, "EFFORT");
      break;
  }
}

void GenericJoint::read(const ros::Duration &period)
{
  std::map<GenericHardware::Mode, std::shared_ptr<int>> map;
  std::vector<std::pair<::GenericHardwareParameter, int*>> actual;
  for (GenericHardware::Mode mode : this->hardware_->getSupportedActualModes())
  {
    map.insert({mode, std::make_shared<int>()});
    actual.push_back({this->hardware_->getActualParameterForMode(mode), map.at(mode).get()});
  }

  this->connection_->read(this->id_, actual);

  for (GenericHardware::Mode mode : this->hardware_->getSupportedActualModes())
  {
    if (mode == GenericHardware::Mode::POSITION)
      this->actual_position_ = this->hardware_->convertFromHardwareResolution(*map.at(mode), mode);
    if (mode == GenericHardware::Mode::VELOCITY)
    {
      if (this->diameter_ > 0.0)
        this->actual_velocity_ = this->RadPStoMPS(this->hardware_->convertFromHardwareResolution(*map.at(mode), mode));
      else
        this->actual_velocity_ = this->hardware_->convertFromHardwareResolution(*map.at(mode), mode);
    }
    if (mode == GenericHardware::Mode::EFFORT)
      this->actual_effort_ = this->hardware_->convertFromHardwareResolution(*map.at(mode), mode);
  }
}

void GenericJoint::write(GenericHardwareParameter hardware_parameter, int data)
{
  this->connection_->write(this->id_, hardware_parameter, data);
}

int GenericJoint::read(GenericHardwareParameter hardware_parameter)
{
  return this->connection_->read(this->id_, hardware_parameter);
}

bool GenericJoint::setMode(GenericHardware::Mode mode)
{
  if (this->hardware_->supportsTargetMode(mode))
  {
    this->mode_ = std::make_unique<GenericHardware::Mode>(mode);
    ROS_INFO("[%s] SUCCESS setting mode %s on joint %s",
             PREFIX, GenericHardware::modeToString(mode).LOG, this->name_.LOG);
    return true;
  }
  else
  {
    ROS_WARN("[%s] ERROR setting mode %s on joint %s - this mode is not supported by %s",
             PREFIX, GenericHardware::modeToString(mode).LOG, this->name_.LOG, this->hardware_->getName().LOG);
    return false;
  }
}

JointStateHandle *GenericJoint::getJointStateHandle()
{
  return this->joint_state_handle_.get();
}

JointHandle *GenericJoint::getJointPositionHandle()
{
  return this->joint_position_handle_.get();
}

JointHandle *GenericJoint::getJointVelocityHandle()
{
  return this->joint_velocity_handle_.get();
}

JointHandle *GenericJoint::getJointEffortHandle()
{
  return this->joint_effort_handle_.get();
}

void GenericJoint::writeTarget(double target, GenericHardware::Mode mode, const std::string& mode_name)
{
  if (this->hardware_->supportsTargetMode(mode))
  {
    if (mode == GenericHardware::Mode::POSITION)
    {
      int hardware_target = this->hardware_->convertToHardwareResolution(target, mode);
      this->connection_->write(this->id_, this->hardware_->getTargetParameterForMode(mode), hardware_target);
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
      this->connection_->write(this->id_, this->hardware_->getTargetParameterForMode(mode), hardware_target);
    }
    if (mode == GenericHardware::Mode::EFFORT)
    {
      int hardware_target = this->hardware_->convertToHardwareResolution(target, mode);
      this->connection_->write(this->id_, this->hardware_->getTargetParameterForMode(mode), hardware_target);
    }
  }
  else
  {
    ROS_WARN("[%s] %s is not supporting target mode %s", PREFIX, this->hardware_->getName().LOG, mode_name.LOG);
    throw std::runtime_error("unsupported target mode requested");
  }
}

double GenericJoint::readActual(GenericHardware::Mode mode, const std::string& mode_name)
{
  if (this->hardware_->supportsActualMode(mode))
  {
    int hardware_actual = this->connection_->read(this->id_, this->hardware_->getActualParameterForMode(mode));
    return this->hardware_->convertFromHardwareResolution(hardware_actual, mode);
  }
  else
  {
    ROS_WARN("[%s] %s is not supporting target mode %s", PREFIX, this->hardware_->getName().LOG, mode_name.LOG);
    throw std::runtime_error("unsupported actual mode requested");
  }
}

double GenericJoint::MPStoRadPS(double mps)
{
  return mps / (this->diameter_  / 2.0);
}

double GenericJoint::RadPStoMPS(double rps)
{
  return (this->diameter_ / 2.0) * rps;
}
