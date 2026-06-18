// Copyright 2022 Eugen Kaltenegger

#ifndef TUW_HARDWARE_INTERFACE_TEMPLATE_GENERIC_JOINT_H
#define TUW_HARDWARE_INTERFACE_TEMPLATE_GENERIC_JOINT_H

#include <memory>
#include <string>

#include <ros/package.h>
#include <joint_limits_interface/joint_limits.h>
#include <joint_limits_interface/joint_limits_urdf.h>
#include <joint_limits_interface/joint_limits_interface.h>

#include <tuw_hardware_interface_template/generic_hardware.h>

using hardware_interface::JointHandle;
using hardware_interface::JointStateHandle;

using joint_limits_interface::JointLimits;
using joint_limits_interface::SoftJointLimits;

using joint_limits_interface::PositionJointSaturationHandle;
using joint_limits_interface::VelocityJointSaturationHandle;
using joint_limits_interface::EffortJointSaturationHandle;

using joint_limits_interface::PositionJointSoftLimitsHandle;
using joint_limits_interface::VelocityJointSoftLimitsHandle;
using joint_limits_interface::EffortJointSoftLimitsHandle;

using tuw_hardware_interface::GenericHardware;

namespace tuw_hardware_interface
{
class GenericConfig;
class GenericConnection;
class GenericHardwareParameter;
class GenericJointDescription;
class GenericJoint
{
public:
  explicit GenericJoint(GenericJointDescription joint_description);
  GenericJoint() = default;
  ~GenericJoint() = default;

  virtual void setConnection(std::shared_ptr<GenericConnection> connection);
  virtual void setHardware(std::shared_ptr<GenericHardware> hardware);
  virtual void setConfig(std::shared_ptr<GenericConfig> config);

  virtual std::string getName();
  virtual int getId();
  virtual int getDiameter();

  virtual void write(const ros::Duration &period);
  virtual void read(const ros::Duration &period);

  virtual void write(GenericHardwareParameter hardware_parameter, int data);
  virtual int read(GenericHardwareParameter hardware_parameter);

  virtual bool setMode(GenericHardware::Mode mode);

  double MPStoRadPS(double mps);
  double RadPStoMPS(double rps);

  JointStateHandle* getJointStateHandle();
  JointHandle* getJointPositionHandle();
  JointHandle* getJointVelocityHandle();
  JointHandle* getJointEffortHandle();
protected:
  virtual void writeTarget(double target, GenericHardware::Mode mode, const std::string& mode_name);
  virtual double readActual(GenericHardware::Mode mode, const std::string& mode_name);

  std::string name_;
  int id_ {0};
  double diameter_ {0};

  std::shared_ptr<GenericConnection> connection_;
  std::shared_ptr<GenericHardware> hardware_;
  std::shared_ptr<GenericConfig> config_;

  std::unique_ptr<GenericHardware::Mode> mode_;

  // target values
  double target_position_ {0.0};
  double target_velocity_ {0.0};
  double target_effort_ {0.0};
  // actual values
  double actual_position_ {0.0};
  double actual_velocity_ {0.0};
  double actual_effort_ {0.0};
  // state handle
  std::unique_ptr<JointStateHandle> joint_state_handle_ {nullptr};
  // command handles
  std::unique_ptr<JointHandle> joint_position_handle_ {nullptr};
  std::unique_ptr<JointHandle> joint_velocity_handle_ {nullptr};
  std::unique_ptr<JointHandle> joint_effort_handle_ {nullptr};
  // limit container
  std::unique_ptr<JointLimits> limits_ {std::make_unique<JointLimits>()};
  std::unique_ptr<SoftJointLimits> soft_limits_ {std::make_unique<SoftJointLimits>()};
  // limit handles
  std::unique_ptr<PositionJointSaturationHandle> joint_position_limit_handle_ {nullptr};
  std::unique_ptr<VelocityJointSaturationHandle> joint_velocity_limit_handle_ {nullptr};
  std::unique_ptr<EffortJointSaturationHandle> joint_effort_limit_handle_ {nullptr};
  std::unique_ptr<PositionJointSoftLimitsHandle> joint_position_soft_limit_handle_ {nullptr};
  std::unique_ptr<VelocityJointSoftLimitsHandle> joint_velocity_soft_limit_handle_ {nullptr};
  std::unique_ptr<EffortJointSoftLimitsHandle> joint_effort_soft_limit_handle_ {nullptr};
};
}  // namespace tuw_hardware_interface

#endif  // TUW_HARDWARE_INTERFACE_TEMPLATE_GENERIC_JOINT_H
