// Copyright 2022 Eugen Kaltenegger

#include <ros/ros.h>
#include <controller_manager/controller_manager.h>
#include <tuw_hardware_interface_template/generic_setup_prefix.h>
#include <tuw_hardware_interface_trinamic/trinamic_hardware_interface.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "tuw_hardware_interface_trinamic");
  ros::NodeHandle basic_nh;
  ros::NodeHandle hardware_nh("/hardware");
  ros::NodeHandle controller_nh("/controller");
  int control_loop_hz;
  basic_nh.param("tuw_hardware_interface_dynamixel_loop_hz", control_loop_hz, 300);
  ros::Rate rate(control_loop_hz);

  ros::AsyncSpinner spinner(1);
  spinner.start();

  tuw_hardware_interface::GenericSetupPrefix::setSetupName("test_setup");
  tuw_hardware_interface::TrinamicHardwareInterface trinamic_hardware_interface;
  if (trinamic_hardware_interface.init(basic_nh, hardware_nh))
  {
    ROS_INFO("[%s] SUCCESS initializing", PREFIX);
    ROS_INFO("[%s] control loop will operate at %d hz", PREFIX, control_loop_hz);
  }
  else
  {
    ROS_ERROR("[%s] ERROR initializing", PREFIX);
    ROS_ERROR("[%s] shutting down ...", PREFIX);
    ros::shutdown();
  }
  controller_manager::ControllerManager controller_manager(&trinamic_hardware_interface, controller_nh);
  ros::Time update_time;
  ros::Time last_update_time = ros::Time::now();
  ros::Duration duration;
  ROS_INFO("[%s] control loop started", PREFIX);
  while (ros::ok())
  {
    update_time = ros::Time::now();
    duration = update_time - last_update_time;

    trinamic_hardware_interface.read(update_time, duration);
    controller_manager.update(update_time, duration);
    trinamic_hardware_interface.write(update_time, duration);

    last_update_time = update_time;
    rate.sleep();
  }
}
