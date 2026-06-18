// Copyright 2022 Eugen Kaltenegger

#ifndef DIP_WS_TRINAMIC_HARDWARE_H
#define DIP_WS_TRINAMIC_HARDWARE_H

#include <tuw_hardware_interface_template/generic_hardware.h>
#include <tuw_hardware_interface_trinamic/description/trinamic_hardware_description.h>
#include <tuw_hardware_interface_trinamic/trinamic_hardware_parameter.h>

namespace tuw_hardware_interface
{
class TrinamicHardware : public GenericHardware
{
public:
  static std::shared_ptr<TrinamicHardware> getHardware(TrinamicHardwareDescription hardware_description);
  explicit TrinamicHardware(TrinamicHardwareDescription hardware_description);
  bool supportsTargetMode(Mode mode) override;
  bool supportsActualMode(Mode mode) override;
  std::vector<Mode> getSupportedTargetModes() override;
  std::vector<Mode> getSupportedActualModes() override;
  TrinamicHardwareParameter getTargetTrinamicParameterForMode(Mode mode);
  TrinamicHardwareParameter getActualTrinamicParameterForMode(Mode mode);
  std::shared_ptr<std::map<std::string, TrinamicHardwareParameter>> getConfigIdentifierToTrinamicParameter();
protected:
  static std::unique_ptr<std::map<std::string, std::shared_ptr<TrinamicHardware>>> trinamic_hardware_table_;
  std::map<Mode, TrinamicHardwareParameter> target_modes_to_trinamic_parameter_;
  std::map<Mode, TrinamicHardwareParameter> actual_modes_to_trinamic_parameter_;
  std::shared_ptr<std::map<std::string, TrinamicHardwareParameter>> config_identifier_to_trinamic_parameter_;
};
}  // namespace tuw_hardware_interface

#endif //DIP_WS_TRINAMIC_HARDWARE_H
