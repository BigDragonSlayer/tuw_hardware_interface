// Copyright 2022 Eugen Kaltenegger

#include <tuw_hardware_interface_template/generic_hardware_interface.h>

#include <list>
#include <memory>
#include <string>

#include <tuw_hardware_interface_template/description/generic_joint_description.h>
#include <tuw_hardware_interface_template/generic_setup_prefix.h>
#include <tuw_hardware_interface_template/description/generic_setup_description.h>

#include <tuw_hardware_interface_template/generic_config.h>

using tuw_hardware_interface::GenericJoint;
using tuw_hardware_interface::GenericSetupDescription;
using tuw_hardware_interface::GenericHardwareInterface;

std::string GenericHardwareInterface::setup_parameter_ = "generic_setup";

bool GenericHardwareInterface::init(ros::NodeHandle &basic_node_handle,
                                    ros::NodeHandle &hardware_node_handle)
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
    GenericSetupDescription setup_description = GenericSetupDescription(setup_yaml);
    GenericSetupPrefix::setSetupName(setup_description.getName());
    this->initJoints(setup_description.getJoints());
    return true;
  }
  catch (std::exception &exception)
  {
    ROS_ERROR("[%s] ERROR INITIALIZING NODE: %s", PREFIX, exception.what());
    return false;
  }
}

void GenericHardwareInterface::write(const ros::Time &time, const ros::Duration &period)
{
  for (const auto& joint : this->joints_)
  {
    joint->write(period);
  }
}

void GenericHardwareInterface::read(const ros::Time &time, const ros::Duration &period)
{
  for (const auto& joint : this->joints_)
  {
    joint->read(period);
  }
}

void GenericHardwareInterface::doSwitch(const std::list<hardware_interface::ControllerInfo> &start_list,
                                        const std::list<hardware_interface::ControllerInfo> &stop_list)
{
  for (const auto& controller_info : start_list)
  {
    for (const auto& hardware_interface : controller_info.claimed_resources)
    {
      for (const auto& hardware_resource : hardware_interface.resources)
      {
        std::shared_ptr<GenericJoint> joint = this->findJoint(hardware_resource);
        if (joint)
        {
          ROS_INFO("[%s] setting joint %s to operation mode: %s",
                   PREFIX, joint->getName().LOG, controller_info.type.LOG);
          joint->setMode(GenericHardware::modeFromString(controller_info.type));
        }
        else
        {
          ROS_DEBUG("[%s] the joint %s is not part of the tuw_dynamixel interface",
                    PREFIX, joint->getName().LOG);
        }
      }
    }
  }
}

bool GenericHardwareInterface::initJoints(std::list<GenericJointDescription> joint_descriptions)
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

bool GenericHardwareInterface::initJoint(GenericJointDescription joint_description)
{
  try
  {
    std::shared_ptr<GenericJoint> joint = std::make_shared<GenericJoint>(joint_description);

    // create connection and set it to joint
    // create hardware and set it to joint
    // create config and set it to joint

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

std::shared_ptr<GenericJoint> GenericHardwareInterface::findJoint(const std::string &name)
{
  for (const auto &joint : this->joints_)
  {
    if (joint->getName() == name) return joint;
  }
  throw std::runtime_error("there is no joint with the name " + name);
}
