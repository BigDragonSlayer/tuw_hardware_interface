// Copyright 2022 Eugen Kaltenegger

#ifndef DIP_WS_TRINAMIC_HARDWARE_DESCRIPTION_H
#define DIP_WS_TRINAMIC_HARDWARE_DESCRIPTION_H

#include "tuw_hardware_interface_template/description/generic_hardware_description.h"
#include "trinamic_hardware_parameter_description.h"

namespace tuw_hardware_interface
{
class TrinamicHardwareDescription : public GenericHardwareDescription
{
public:
  explicit TrinamicHardwareDescription(YAML::Node yaml);
  std::shared_ptr<std::map<std::string, TrinamicHardwareParameterDescription>> getTargetIdentifierToTrinamicDescription();
  std::shared_ptr<std::map<std::string, TrinamicHardwareParameterDescription>> getActualIdentifierToTrinamicDescription();
  std::shared_ptr<std::map<std::string, TrinamicHardwareParameterDescription>> getConfigIdentifierToTrinamicDescription();

protected:
  std::shared_ptr<std::map<std::string, TrinamicHardwareParameterDescription>>
          target_identifier_to_trinamic_description_{nullptr};
  std::shared_ptr<std::map<std::string, TrinamicHardwareParameterDescription>>
          actual_identifier_to_trinamic_description_{nullptr};
  std::shared_ptr<std::map<std::string, TrinamicHardwareParameterDescription>>
          config_identifier_to_trinamic_description_{nullptr};
};
}  // namespace tuw_hardware_interface

#endif //DIP_WS_TRINAMIC_HARDWARE_DESCRIPTION_H
