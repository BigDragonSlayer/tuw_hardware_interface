cmake_minimum_required(VERSION 3.0.2)
project(tuw_hardware_interface_trinamic)

add_compile_options(-std=c++14)

link_libraries(yaml-cpp)

find_package(catkin
  REQUIRED COMPONENTS
  roscpp
  rospy
  roslib
  roslint
  urdf
  tuw_hardware_interface_template
  ddynamic_reconfigure
  controller_manager
  hardware_interface
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS
  roscpp
  rospy
  roslib
  roslint
  urdf
  ddynamic_reconfigure
  tuw_hardware_interface_template
  controller_manager
  hardware_interface
)

include_directories(
 ./include
 ${catkin_INCLUDE_DIRS}
)

roslint_cpp()

add_library(
  tuw_hardware_interface_trinamic
  src/${PROJECT_NAME}/description/trinamic_hardware_description.cpp
  src/${PROJECT_NAME}/description/trinamic_hardware_parameter_description.cpp
  src/${PROJECT_NAME}/description/trinamic_joint_description.cpp
  src/${PROJECT_NAME}/description/trinamic_setup_description.cpp
  src/${PROJECT_NAME}/trinamic_connection/trinamic_message/trinamic_command.cpp
  src/${PROJECT_NAME}/trinamic_connection/trinamic_message/trinamic_message.cpp
  src/${PROJECT_NAME}/trinamic_connection/trinamic_message/trinamic_reply.cpp
  src/${PROJECT_NAME}/trinamic_connection/tmcm1640_connection.cpp
  src/${PROJECT_NAME}/trinamic_config.cpp
  src/${PROJECT_NAME}/trinamic_hardware.cpp
  src/${PROJECT_NAME}/trinamic_hardware_interface.cpp
  src/${PROJECT_NAME}/trinamic_hardware_parameter.cpp
  src/${PROJECT_NAME}/trinamic_joint.cpp
)

target_link_libraries(
  tuw_hardware_interface_trinamic
  ${catkin_LIBRARIES}
)

catkin_add_gtest(
  ${PROJECT_NAME}_tests
  test/${PROJECT_NAME}/trinamic_connection/trinamic_message/trinamic_command_test.cpp
  test/${PROJECT_NAME}/trinamic_connection/trinamic_message/trinamic_message_test.cpp
  test/${PROJECT_NAME}/trinamic_connection/trinamic_message/trinamic_reply_test.cpp
  test/${PROJECT_NAME}/all_tests.cpp
)

target_link_libraries(
  ${PROJECT_NAME}_tests
  ${PROJECT_NAME}
  gmock
)

add_executable(
  ${PROJECT_NAME}_node
  src/trinamic_hardware_interface_node.cpp
)

add_dependencies(
  ${PROJECT_NAME}_node
  ${PROJECT_NAME}
)

target_link_libraries(
  ${PROJECT_NAME}_node
  ${PROJECT_NAME}
  ${catkin_LIBRARIES}
)
