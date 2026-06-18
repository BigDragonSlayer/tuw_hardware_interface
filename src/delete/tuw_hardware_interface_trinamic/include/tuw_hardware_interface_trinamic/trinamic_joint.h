// Copyright 2023 Eugen Kaltenegger

#ifndef DIP_WS_TRINAMIC_JOINT_H
#define DIP_WS_TRINAMIC_JOINT_H

#include "trinamic_hardware_parameter.h"
#include "trinamic_hardware.h"
#include "tuw_hardware_interface_trinamic/trinamic_connection/tmcm1640_connection.h"
#include "tuw_hardware_interface_template/generic_joint.h"

namespace tuw_hardware_interface
{
class TrinamicJoint : public GenericJoint
{
public:
  explicit TrinamicJoint(GenericJointDescription joint_description);
  void setTrinamicConnection(std::shared_ptr<TMCM1640Connection> connection);
  void setTrinamicHardware(std::shared_ptr<TrinamicHardware> hardware);

  void writeTrinamic(TrinamicHardwareParameter hardware_parameter, int data);
  int readTrinamic(TrinamicHardwareParameter hardware_parameter);

  void write(const ros::Duration &period) override;
  void read(const ros::Duration &period) override;
protected:
  void writeTarget(double target, GenericHardware::Mode mode, const std::string& mode_name);
  double readActual(GenericHardware::Mode mode, const std::string& mode_name);

  std::shared_ptr<TrinamicHardware> hardware_;
  std::shared_ptr<TMCM1640Connection> connection_;
};
}  // namespace tuw_hardware_interface

#endif //DIP_WS_TRINAMIC_JOINT_H
