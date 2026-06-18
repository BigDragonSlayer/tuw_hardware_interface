// Copyright 2022 Eugen Kaltenegger

#ifndef DIP_WS_TRINAMIC_SETUP_DESCRIPTION_H
#define DIP_WS_TRINAMIC_SETUP_DESCRIPTION_H

#include "tuw_hardware_interface_template/description/generic_setup_description.h"
#include <tuw_hardware_interface_trinamic/description/trinamic_joint_description.h>

namespace tuw_hardware_interface
{
class TrinamicSetupDescription : public GenericSetupDescription
{
public:
  explicit TrinamicSetupDescription(YAML::Node yaml);
  std::list<TrinamicJointDescription> getTrinamicJoints();
protected:
  std::list<TrinamicJointDescription> trinamic_joints_ {std::list<TrinamicJointDescription>()};
};
}  // namespace tuw_hardware_interface

#endif //DIP_WS_TRINAMIC_SETUP_DESCRIPTION_H
