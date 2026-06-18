// Copyright 2023 Eugen Kaltenegger

#ifndef DIP_WS_TRINAMIC_CONFIG_H
#define DIP_WS_TRINAMIC_CONFIG_H

#include "tuw_hardware_interface_template/generic_config.h"
#include "trinamic_hardware.h"
#include "trinamic_joint.h"

namespace tuw_hardware_interface
{
class TrinamicConfig : public GenericConfig
{
public:
  TrinamicConfig(std::shared_ptr<TrinamicJoint> joint,
                 std::shared_ptr<TrinamicHardware> hardware);
  TrinamicConfig(std::shared_ptr<TrinamicJoint> joint,
                 std::shared_ptr<TrinamicHardware> hardware,
                 GenericConfigDescription config_description);
protected:
  void setInitialConfig(GenericConfigDescription config_description) override;
  void reconfigureValue(TrinamicHardwareParameter parameter, int target_value);
  void setupReconfigureServer() override;
  void reconfigureConfig() override;
  std::shared_ptr<TrinamicJoint> joint_;
  std::shared_ptr<TrinamicHardware> trinamic_hardware_ {nullptr};
};
}  // namespace tuw_hardware_interface

#endif //DIP_WS_TRINAMIC_CONFIG_H
