// Copyright 2022 Eugen Kaltenegger

#include "tuw_hardware_interface_template/generic_setup_prefix.h"
#include "tuw_hardware_interface_template/generic_config.h"
#include <tuw_hardware_interface_trinamic/trinamic_hardware_interface.h>
#include "tuw_hardware_interface_trinamic/description/trinamic_setup_description.h"
#include "tuw_hardware_interface_trinamic/trinamic_connection/tmcm1640_connection.h"
#include "tuw_hardware_interface_trinamic/trinamic_hardware.h"
#include "tuw_hardware_interface_trinamic/trinamic_config.h"
#include "tuw_hardware_interface_trinamic/trinamic_joint.h"

using tuw_hardware_interface::TrinamicHardware;
using tuw_hardware_interface::TrinamicHardwareInterface;
using tuw_hardware_interface::GenericHardwareInterface;
using tuw_hardware_interface::TrinamicJointDescription;

std::string GenericHardwareInterface::setup_parameter_ = "trinamic_hardware_interface_setup";

bool TrinamicHardwareInterface::init(ros::NodeHandle& basic_node_handle,
                                                             ros::NodeHandle& hardware_node_handle)
{
  std::string setup_file_path;
  YAML::Node setup_yaml;

  if (!basic_node_handle.hasParam(GenericHardwareInterface::setup_parameter_))
  {
    ROS_ERROR("[%s] parameter %s is not set", PREFIX, GenericHardwareInterface::setup_parameter_.LOG);
    return false;
  }

  try
  {
    basic_node_handle.getParam(GenericHardwareInterface::setup_parameter_, setup_file_path);
  }
  catch(...)
  {
    ROS_ERROR("[%s] PARAMETER FOR SETUP FILE NOT FOUND (\"%s\")",
              PREFIX, GenericHardwareInterface::setup_parameter_.LOG);
    return false;
  }

  try
  {
    setup_yaml = YAML::LoadFile(setup_file_path);
  }
  catch (...)
  {
    ROS_ERROR("[%s] SETUP FILE NOT FOUND OR INVALID (\"%s\")",
              PREFIX, setup_file_path.LOG);
    return false;
  }

  try
  {
    TrinamicSetupDescription setup_description = TrinamicSetupDescription(setup_yaml);
    GenericSetupPrefix::setSetupName(setup_description.getName());
    this->initJoints(setup_description.getTrinamicJoints());
    return true;
  }
  catch (std::exception &exception)
  {
    ROS_ERROR("[%s] ERROR INITIALIZING NODE: %s", PREFIX, exception.what());
    return false;
  }
}

bool TrinamicHardwareInterface::initJoints(std::list<TrinamicJointDescription> joint_descriptions)
{
  try
  {
    for (const auto& joint_description : joint_descriptions)
    {
      this->initJoint(joint_description);
    }

    registerInterface(&this->joint_state_interface);
    registerInterface(&this->joint_position_interface);
    registerInterface(&this->joint_velocity_interface);
    registerInterface(&this->joint_effort_interface);

    ROS_INFO("[%s] FINISH setting up %zu joint(s)", PREFIX, this->joints_.size());

    return true;
  }
  catch(...)
  {
    return false;
  }
}

bool TrinamicHardwareInterface::initJoint(
        tuw_hardware_interface::TrinamicJointDescription joint_description)
{
  try
  {
    std::shared_ptr<TrinamicJoint> joint = std::make_shared<TrinamicJoint>(joint_description);

    // connection
    std::shared_ptr<GenericConnectionDescription> connection_description =
            joint_description.getConnectionDescription();
    std::shared_ptr<TMCM1640Connection> connection =
            TMCM1640Connection::getConnection(connection_description);
    joint->setTrinamicConnection(connection);

    // hardware
    std::shared_ptr<TrinamicHardwareDescription> hardware_description =
            joint_description.getTrinamicHardwareDescription();
    std::shared_ptr<TrinamicHardware> hardware =
            TrinamicHardware::getHardware(*hardware_description);
    joint->setTrinamicHardware(hardware);

    // config
    std::shared_ptr<GenericConfigDescription> config_description =
            joint_description.getConfigDescription();
    std::shared_ptr<TrinamicConfig> config =
            std::make_shared<TrinamicConfig>(joint, hardware, *config_description);
    joint->setConfig(config);

    this->joints_.push_back(joint);

    this->joint_state_interface.registerHandle(*joint->getJointStateHandle());
    this->joint_position_interface.registerHandle(*joint->getJointPositionHandle());
    this->joint_velocity_interface.registerHandle(*joint->getJointVelocityHandle());
    this->joint_effort_interface.registerHandle(*joint->getJointEffortHandle());

    ROS_INFO("[%s] FINISH setting up joint %s", PREFIX, joint->getName().LOG);

    return true;
  }
  catch(...)
  {
    return false;
  }
}

PLUGINLIB_EXPORT_CLASS(tuw_hardware_interface::TrinamicHardwareInterface, hardware_interface::RobotHW)
