cmake_minimum_required(VERSION 3.8)
project(tuw_hardware_trinamic_interface)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
find_package(rclcpp REQUIRED)

# for the trinamic interface
find_package(hardware_interface REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)

add_library(tuw_hardware_trinamic_interface SHARED
  src/tuw_hardware_trinamic_interface.cpp
)

target_include_directories(tuw_hardware_trinamic_interface PUBLIC
  include
)

target_link_libraries(tuw_hardware_trinamic_interface PUBLIC
  hardware_interface::hardware_interface
  pluginlib::pluginlib
  rclcpp::rclcpp
  rclcpp_lifecycle::rclcpp_lifecycle
)

pluginlib_export_plugin_description_file(hardware_interface trinamic_interface.xml)

install(
  TARGETS tuw_hardware_trinamic_interface
  DESTINATION lib
)

install(DIRECTORY
  include
  DESTINATION include
)

# for the testnode
include_directories(include)

add_executable(trinamic_test_node
  src/test_node.cpp
  src/tuw_hardware_trinamic_connection.cpp
)
ament_target_dependencies(trinamic_test_node rclcpp)

install(DIRECTORY
  include
  DESTINATION include
)

install(TARGETS
  trinamic_test_node
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
