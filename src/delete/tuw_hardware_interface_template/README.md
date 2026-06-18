# `tuw_hardware_interface_template`

## Description

This package contains a library serving as a template for hardware operation with [`ros_control`](https://wiki.ros.org/ros_control) and [`ros_controllers`](https://wiki.ros.org/ros_controllers).
The documentation for `ros_control` is not very detailed.
For this reason I recommend reading [this blog post](https://wiki.ros.org/ros_controllers) to get a better understanding of `ros_control`.
Additionally, this library supports a reconfigure server to configure the hardware.
This makes use of the [`dynamic_reconfigure`](https://wiki.ros.org/dynamic_reconfigure) and the [`ddynamic_reconfigure`](https://github.com/pal-robotics/ddynamic_reconfigure) packages.

This library aims to provide high flexibility provided by a number of `yaml` files.
A setup is defined with a setup name and a description for each joint.
For each joint a name, an ID, connection parameters as well as path to the hardware description `yaml` file and the hardware configuration description `yaml` file are defined.
The hardware description is designed to define hardware operation, by providing hardware parameters.
The configuration description is designed to define hardware configuration, which are specific values for the hardware parameters.

In this package each part parsed from a `yaml` file is first converted to `Generic<Thing>Description` object.
This object is then an argument for the `Generic<Thing>` object.
In order to see the intended structure of the `yaml` files check out the `yaml` files for testing, these are stored in `test/resources/`.

For an example of a hardware specific implementation check out the `tuw_hardware_interface_dynamixel` and `tuw_hardware_interface_trinamic` packages. 

## Application

This package contains various tests as well as a code coverage report.
Beside these there are no executables in this package.

**Run tests:**
```bash
catkin_make -DCMAKE_BUILD_TYPE=Debug
catkin_make run_tests
```

**Run code coverage check:**
```bash
catkin_make -DCMAKE_BUILD_TYPE=Debug
catkin_make -DENABLE_COVERAGE_TESTING=ON -DCMAKE_BUILD_TYPE=Debug tuw_hardware_interface_template_coverage_report
```
