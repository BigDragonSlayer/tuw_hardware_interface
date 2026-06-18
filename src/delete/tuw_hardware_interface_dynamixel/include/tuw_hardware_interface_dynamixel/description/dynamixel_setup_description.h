// Copyright 2022 Eugen Kaltenegger

#ifndef TUW_HARDWARE_INTERFACE_DYNAMIXEL_DESCRIPTION_DYNAMIXEL_SETUP_DESCRIPTION_H
#define TUW_HARDWARE_INTERFACE_DYNAMIXEL_DESCRIPTION_DYNAMIXEL_SETUP_DESCRIPTION_H

#include <tuw_hardware_interface_template/description/generic_setup_description.h>

#include <list>

namespace tuw_hardware_interface
{
class DynamixelJointDescription;
class DynamixelSetupDescription : public GenericSetupDescription
{
public:
  explicit DynamixelSetupDescription(YAML::Node yaml);
  std::list<DynamixelJointDescription> getDynamixelJoints();
protected:
  std::list<DynamixelJointDescription> dynamixel_joints_ {std::list<DynamixelJointDescription>()};
};
}  // namespace tuw_hardware_interface

#endif  // TUW_HARDWARE_INTERFACE_DYNAMIXEL_DESCRIPTION_DYNAMIXEL_SETUP_DESCRIPTION_H
