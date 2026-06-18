// Copyright 2022 Eugen Kaltenegger

#ifndef DIP_WS_TRINAMIC_HARDWARE_PARAMETER_H
#define DIP_WS_TRINAMIC_HARDWARE_PARAMETER_H

#include "tuw_hardware_interface_template/generic_hardware_parameter.h"
#include "tuw_hardware_interface_trinamic/description/trinamic_hardware_parameter_description.h"

namespace tuw_hardware_interface
{
class TrinamicHardwareParameter : public GenericHardwareParameter
{
public:
  explicit TrinamicHardwareParameter(TrinamicHardwareParameterDescription hardware_parameter_description);
  std::shared_ptr<int> getParameter();
protected:
  bool isValid() override;
  bool isTarget() override;
  bool isActual() override;
  bool isRange() override;
  bool isEnum() override;
  std::shared_ptr<int> parameter_ {nullptr};
};
}  // namespace tuw_hardware_interface

#endif //DIP_WS_TRINAMIC_HARDWARE_PARAMETER_H
