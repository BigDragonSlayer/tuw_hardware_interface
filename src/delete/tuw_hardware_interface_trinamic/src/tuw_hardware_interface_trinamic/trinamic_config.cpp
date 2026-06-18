// Copyright 2023 Eugen Kaltenegger

#include "tuw_hardware_interface_trinamic/trinamic_config.h"
#include "tuw_hardware_interface_template/generic_setup_prefix.h"
#include "tuw_hardware_interface_template/generic_joint.h"
#include "tuw_hardware_interface_trinamic/trinamic_joint.h"

using tuw_hardware_interface::TrinamicConfig;
using tuw_hardware_interface::GenericConfigDescription;

TrinamicConfig::TrinamicConfig(std::shared_ptr<TrinamicJoint> joint,
                               std::shared_ptr<TrinamicHardware> hardware)
        : joint_(std::move(joint)),
          trinamic_hardware_(std::move(hardware))
{
  this->setupReconfigureServer();
}

TrinamicConfig::TrinamicConfig(std::shared_ptr<TrinamicJoint> joint,
                               std::shared_ptr<TrinamicHardware> hardware,
                               tuw_hardware_interface::GenericConfigDescription config_description)
        : joint_(std::move(joint)),
          trinamic_hardware_(std::move(hardware))
{
  this->setupReconfigureServer();
  this->setInitialConfig(std::move(config_description));
}

void tuw_hardware_interface::TrinamicConfig::setupReconfigureServer()
{
  ros::NodeHandle node_handle(GenericSetupPrefix::getNodeName() + std::string("/") + this->joint_->getName());
  this->reconfigure_ = std::make_unique<ddynamic_reconfigure::DDynamicReconfigure>(node_handle);

  auto config_identifier_to_trinamic_parameter = this->trinamic_hardware_->getConfigIdentifierToTrinamicParameter();

  for (const auto& config_identifier : *this->trinamic_hardware_->getConfigIdentifiers())
  {
    auto parameter = config_identifier_to_trinamic_parameter->at(config_identifier);
    this->registerReconfigureVariable(parameter);
    this->actual_config_values_[config_identifier] = this->joint_->readTrinamic(parameter);
  }

  auto callback = boost::bind(&TrinamicConfig::reconfigureConfig, this);
  this->reconfigure_->setUserCallback(callback);
  this->reconfigure_->publishServicesTopics();
}

void tuw_hardware_interface::TrinamicConfig::setInitialConfig(GenericConfigDescription config_description)
{
  for (const auto& identifier_value_pair : config_description.getConfig())
  {
    auto identifier = identifier_value_pair.first;

    auto config_identifiers_iterator = std::find(this->trinamic_hardware_->getConfigIdentifiers()->begin(),
                                                 this->trinamic_hardware_->getConfigIdentifiers()->end(),
                                                 identifier);

    if (config_identifiers_iterator != this->trinamic_hardware_->getConfigIdentifiers()->end())
    {
      auto parameter = this->trinamic_hardware_->getConfigIdentifierToTrinamicParameter()->at(identifier);
      int value = identifier_value_pair.second;
      this->reconfigureValue(parameter, value);
    }
    else
    {
      auto hardware_name = this->trinamic_hardware_->getName();
      ROS_WARN("[%s] the config parameter %s is not valid for %s", PREFIX, identifier.LOG, hardware_name.LOG);
    }
  }
}

void TrinamicConfig::reconfigureValue(TrinamicHardwareParameter parameter, int target_value)
{
  const auto identifier = *parameter.getIdentifier();
  this->joint_->writeTrinamic(parameter, target_value);
  int actual_value = this->joint_->readTrinamic(parameter);

  this->target_config_values_[identifier] = actual_value;
  this->actual_config_values_[identifier] = actual_value;

  if (actual_value == target_value)
    ROS_INFO("[%s] SUCCESS writing parameter %s for joint %s", PREFIX, identifier.LOG, this->joint_->getName().LOG);
  if (actual_value != target_value)
    ROS_WARN("[%s] ERROR writing parameter %s for joint %s", PREFIX, identifier.LOG, this->joint_->getName().LOG);
}

void TrinamicConfig::reconfigureConfig()
{
  for (const std::string& hardware_parameter_identifier : *this->trinamic_hardware_->getConfigIdentifiers())
  {
    int target_value = (this->target_config_values_)[hardware_parameter_identifier];
    int actual_value = (this->actual_config_values_)[hardware_parameter_identifier];

    if (actual_value != target_value)
    {
      ROS_DEBUG("[%s] change in parameter %s", PREFIX, hardware_parameter_identifier.LOG);
      auto hardware_parameter = this->trinamic_hardware_->getConfigIdentifierToTrinamicParameter()->at(hardware_parameter_identifier);
      this->reconfigureValue(hardware_parameter, target_value);
    }
  }
}