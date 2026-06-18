// Copyright 2022 Eugen Kaltenegger

#ifndef DIP_WS_TRINAMIC_JOINT_DESCRIPTION_H
#define DIP_WS_TRINAMIC_JOINT_DESCRIPTION_H

#include "tuw_hardware_interface_template/description/generic_joint_description.h"
#include "trinamic_hardware_description.h"

namespace tuw_hardware_interface
{
class TrinamicJointDescription : public GenericJointDescription
{
public:
  explicit TrinamicJointDescription(const YAML::Node& yaml);
  std::shared_ptr<TrinamicHardwareDescription> getTrinamicHardwareDescription();
private:
  std::shared_ptr<TrinamicHardwareDescription> trinamic_hardware_description_ {nullptr};
};
}  // namespace tuw_hardware_interface

#endif //DIP_WS_TRINAMIC_JOINT_DESCRIPTION_H
