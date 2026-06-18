cmake_minimum_required(VERSION 3.0.2)
project(tuw_hardware_interface_template)

add_compile_options(-std=c++14)

link_libraries(yaml-cpp)

find_package(
  catkin
  REQUIRED COMPONENTS
  roscpp
  rospy
  roslib
  roslint
  urdf
  ddynamic_reconfigure
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
)

if(CATKIN_ENABLE_TESTING AND ENABLE_COVERAGE_TESTING)
  find_package(code_coverage REQUIRED)
  # Add compiler flags for coverage instrumentation before defining any targets
  APPEND_COVERAGE_COMPILER_FLAGS()
endif()

include_directories(
  ./include
  ${catkin_INCLUDE_DIRS}
)

roslint_cpp()

add_library(
  ${PROJECT_NAME}
  src/${PROJECT_NAME}/description/generic_config_description.cpp
  src/${PROJECT_NAME}/description/generic_connection_description.cpp
  src/${PROJECT_NAME}/description/generic_hardware_description.cpp
  src/${PROJECT_NAME}/description/generic_hardware_parameter_description.cpp
  src/${PROJECT_NAME}/description/generic_joint_description.cpp
  src/${PROJECT_NAME}/description/generic_setup_description.cpp
  src/${PROJECT_NAME}/utils/file_loader.cpp
  src/${PROJECT_NAME}/generic_config.cpp
  src/${PROJECT_NAME}/generic_connection.cpp
  src/${PROJECT_NAME}/generic_hardware.cpp
  src/${PROJECT_NAME}/generic_hardware_parameter.cpp
  src/${PROJECT_NAME}/generic_joint.cpp
  src/${PROJECT_NAME}/generic_hardware_interface.cpp
  src/${PROJECT_NAME}/generic_setup_prefix.cpp
)

target_link_libraries(
  ${PROJECT_NAME}
  ${catkin_LIBRARIES}
)

#catkin_add_gtest(
#  ${PROJECT_NAME}_tests
#  test/${PROJECT_NAME}/description/generic_config_description_test.cpp
#  test/${PROJECT_NAME}/description/generic_connection_description_test.cpp
#  test/${PROJECT_NAME}/description/generic_hardware_description_test.cpp
#  test/${PROJECT_NAME}/description/generic_hardware_parameter_description_test.cpp
#  test/${PROJECT_NAME}/description/generic_joint_description_test.cpp
#  test/${PROJECT_NAME}/all_tests.cpp
#  test/${PROJECT_NAME}/generic_config_test.cpp
#  test/${PROJECT_NAME}/generic_connection_test.cpp
#  test/${PROJECT_NAME}/generic_hardware_test.cpp
#  test/${PROJECT_NAME}/generic_hardware_parameter_test.cpp
#  test/${PROJECT_NAME}/generic_joint_test.cpp
#  test/${PROJECT_NAME}/generic_setup_prefix_test.cpp
#)

#target_link_libraries(
#  ${PROJECT_NAME}_tests
#  ${PROJECT_NAME}
#  gmock
#)

if (CATKIN_ENABLE_TESTING)
  catkin_add_gtest(
      ${PROJECT_NAME}_tests
      test/${PROJECT_NAME}/description/generic_config_description_test.cpp
      test/${PROJECT_NAME}/description/generic_connection_description_test.cpp
      test/${PROJECT_NAME}/description/generic_hardware_description_test.cpp
      test/${PROJECT_NAME}/description/generic_hardware_parameter_description_test.cpp
      test/${PROJECT_NAME}/description/generic_joint_description_test.cpp
      test/${PROJECT_NAME}/all_tests.cpp
      test/${PROJECT_NAME}/generic_config_test.cpp
      test/${PROJECT_NAME}/generic_connection_test.cpp
      test/${PROJECT_NAME}/generic_hardware_test.cpp
      test/${PROJECT_NAME}/generic_hardware_parameter_test.cpp
      test/${PROJECT_NAME}/generic_joint_test.cpp
      test/${PROJECT_NAME}/generic_setup_prefix_test.cpp
  )

  target_link_libraries(
    ${PROJECT_NAME}_tests
    ${PROJECT_NAME}
    ${catkin_LIBRARIES}
    gmock
  )

  # Create a target ${PROJECT_NAME}_coverage_report
  if(ENABLE_COVERAGE_TESTING)
    set(COVERAGE_EXCLUDES "*/${PROJECT_NAME}/test*" "*/${PROJECT_NAME}/other_dir_i_dont_care_about*")
    add_code_coverage(
      NAME ${PROJECT_NAME}_coverage_report
      DEPENDENCIES tests
    )
  endif()
endif()

install(
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
)

install(
  DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)
