// Copyright 2022 Eugen Kaltenegger

#include <tuw_hardware_interface_template/generic_config.h>
#include <tuw_hardware_interface_template/generic_connection.h>
#include <tuw_hardware_interface_template/generic_hardware.h>
#include <tuw_hardware_interface_template/generic_joint.h>
#include <tuw_hardware_interface_template/generic_setup_prefix.h>

#include <memory>
#include <utility>
#include <string>

using tuw_hardware_interface::GenericConfig;
using tuw_hardware_interface::GenericConnection;
using tuw_hardware_interface::GenericHardwareParameter;
using tuw_hardware_interface::GenericJoint;

GenericConfig::GenericConfig(std::shared_ptr<GenericJoint> joint,
                             std::shared_ptr<GenericHardware> hardware)
        : joint_(std::move(joint)),
          hardware_(std::move(hardware))
{
  this->setupReconfigureServer();
}

GenericConfig::GenericConfig(std::shared_ptr<GenericJoint> joint,
                             std::shared_ptr<GenericHardware> hardware,
                             GenericConfigDescription config_description)
        : joint_(std::move(joint)),
          hardware_(std::move(hardware))
{
  this->setupReconfigureServer();
  this->setInitialConfig(std::move(config_description));
}

void GenericConfig::setupReconfigureServer()
{
  ros::NodeHandle node_handle(GenericSetupPrefix::getNodeName() + std::string("/") + this->joint_->getName());
  this->reconfigure_ = std::make_unique<ddynamic_reconfigure::DDynamicReconfigure>(node_handle);

  auto config_identifier_to_parameter = this->hardware_->getConfigIdentifierToParameter();

  for (const auto& config_identifier : *this->hardware_->getConfigIdentifiers())
  {
    auto parameter = config_identifier_to_parameter->at(config_identifier);
    this->registerReconfigureVariable(parameter);
    this->actual_config_values_[config_identifier] = this->joint_->read(parameter);
  }

  auto callback = boost::bind(&GenericConfig::reconfigureConfig, this);
  this->reconfigure_->setUserCallback(callback);
  this->reconfigure_->publishServicesTopics();
}

void GenericConfig::registerReconfigureVariable(GenericHardwareParameter hardware_parameter)
{
  if (hardware_parameter.getType() == GenericHardwareParameter::Type::ENUM)
    this->registerReconfigureEnumVariable(hardware_parameter);
  if (hardware_parameter.getType() == GenericHardwareParameter::Type::RANGE)
    this->registerReconfigureRangeVariable(hardware_parameter);
}

void GenericConfig::registerReconfigureEnumVariable(GenericHardwareParameter hardware_parameter)
{
  auto identifier = *hardware_parameter.getIdentifier();
  auto description = *hardware_parameter.getDescription();
  auto enum_map = *hardware_parameter.getEnum();
  int* target = &this->target_config_values_[identifier];
  this->reconfigure_->registerEnumVariable<int>(identifier, target, description, enum_map);
}

void GenericConfig::registerReconfigureRangeVariable(GenericHardwareParameter hardware_parameter)
{
  auto identifier = *hardware_parameter.getIdentifier();
  auto description = *hardware_parameter.getDescription();
  auto minimum = hardware_parameter.getRange()->at("min");
  auto maximum = hardware_parameter.getRange()->at("max");
  int* target = &this->target_config_values_[identifier];
  this->reconfigure_->registerVariable<int>(identifier, target, description, minimum, maximum);
}

void GenericConfig::reconfigureConfig()
{
  for (const std::string& hardware_parameter_identifier : *this->hardware_->getConfigIdentifiers())
  {
    int target_value = (this->target_config_values_)[hardware_parameter_identifier];
    int actual_value = (this->actual_config_values_)[hardware_parameter_identifier];

    if (actual_value != target_value)
    {
      ROS_DEBUG("[%s] change in parameter %s", PREFIX, hardware_parameter_identifier.LOG);
      auto hardware_parameter = this->hardware_->getConfigIdentifierToParameter()->at(hardware_parameter_identifier);
      this->reconfigureValue(hardware_parameter, target_value);
    }
  }
}

void GenericConfig::reconfigureValue(GenericHardwareParameter parameter, int target_value)
{
  const auto identifier = *parameter.getIdentifier();
  this->joint_->write(parameter, target_value);
  int actual_value = this->joint_->read(parameter);

  this->target_config_values_[identifier] = actual_value;
  this->actual_config_values_[identifier] = actual_value;

  if (actual_value == target_value)
    ROS_INFO("[%s] SUCCESS writing parameter %s for joint %s", PREFIX, identifier.LOG, this->joint_->getName().LOG);
  if (actual_value != target_value)
    ROS_WARN("[%s] ERROR writing parameter %s for joint %s", PREFIX, identifier.LOG, this->joint_->getName().LOG);
}

void GenericConfig::setInitialConfig(GenericConfigDescription config_description)
{
  for (const auto& identifier_value_pair : config_description.getConfig())
  {
    auto identifier = identifier_value_pair.first;

    auto config_identifiers_iterator = std::find(this->hardware_->getConfigIdentifiers()->begin(),
                                                 this->hardware_->getConfigIdentifiers()->end(),
                                                 identifier);

    if (config_identifiers_iterator != this->hardware_->getConfigIdentifiers()->end())
    {
      auto parameter = this->hardware_->getConfigIdentifierToParameter()->at(identifier);
      int value = identifier_value_pair.second;
      this->reconfigureValue(parameter, value);
    }
    else
    {
      auto hardware_name = this->hardware_->getName();
      ROS_INFO("[%s] the config parameter %s is not valid for %s", PREFIX, identifier.LOG, hardware_name.LOG);
    }
  }
}
