# `tuw_hardware_interface_dynamixel`

## Description

This package holds an implementation of the `tuw_hardware_interface_template` package, specific to the [Dynamixel MX-106](https://emanual.robotis.com/docs/en/dxl/mx/mx-106/).
For this reason some of the `Generic` classes of the `tuw_hardware_interface_template` have been replaced with `Dynamixel` classes with inheritance.

Note: this package is not ment to be used as standalone, it is rather intended to be used in another package.
For this reason the hardware interface is exported, with the following changes to the `package.xml` file:
```bash
  <export>
    <hardware_interface plugin="${prefix}/tuw_hardware_interface_dynamixel.xml"/>
  </export>
```
Also check the `tuw_hardware_interface_dynamixel.xml`.

## Application

This package contains all that is required to operate a joint with the Dynamixel MX-106.
This includes the following:
- a setup file: `resources/setup/dynamixel_setup.yaml`
- a hardware description file:  `resources/hardware/MX-106.yaml`
- a hardware config file:  `resources/config/MX-106.yaml`
- a robot description (URDF) file: `resources/urdf/simple_dynamixel_robot.yaml` which defines a single joint robot
- a controller definition: `resources/urdf/dynamixel_test_setup.yaml`

These components are all required to start the `launch` file of this package with:
```bash
roslaunch tuw_iwos_hardware_interface_dynamixel tuw_iwos_hardware_interface_dynamixel_node.launch
```
This allows to start the hardware with the aforementioned components.
