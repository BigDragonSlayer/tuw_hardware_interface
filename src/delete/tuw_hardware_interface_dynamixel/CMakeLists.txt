cmake_minimum_required(VERSION 3.0.2)
project(tuw_hardware_interface_dynamixel)

add_compile_options(-std=c++14)

link_libraries(yaml-cpp)

find_package(
  catkin
  REQUIRED COMPONENTS
  roscpp
  rospy
  roslint
  urdf
  tuw_hardware_interface_template
  ddynamic_reconfigure
  controller_manager
  dynamixel_sdk
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS
  tuw_hardware_interface_template
  #  DEPENDS system_lib
)

include_directories(
  ./include
  ${catkin_INCLUDE_DIRS}
)

roslint_cpp()

add_library(
  tuw_hardware_interface_dynamixel
  src/${PROJECT_NAME}/description/dynamixel_connection_description.cpp
  src/${PROJECT_NAME}/description/dynamixel_joint_description.cpp
  src/${PROJECT_NAME}/description/dynamixel_setup_description.cpp
  src/${PROJECT_NAME}/dynamixel_connection.cpp
  src/${PROJECT_NAME}/dynamixel_hardware_interface.cpp
)

target_link_libraries(
  tuw_hardware_interface_dynamixel
  ${catkin_LIBRARIES}
)

add_executable(
  tuw_hardware_interface_dynamixel_node
  src/dynamixel_hardware_interface_node.cpp
)

add_dependencies(
  tuw_hardware_interface_dynamixel_node
  tuw_hardware_interface_dynamixel
)

target_link_libraries(
  tuw_hardware_interface_dynamixel_node
  tuw_hardware_interface_dynamixel
  ${catkin_LIBRARIES}
)
