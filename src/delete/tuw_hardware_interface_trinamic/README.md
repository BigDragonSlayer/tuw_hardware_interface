# `tuw_hardware_interface_trinamic`

## Description

This package holds an implementation of the `tuw_hardware_interface_template` package, specific to the [Trinamic Motor Controller Board TCMC-1640](https://www.trinamic.com/products/modules/details/tmcm-1640/).
For this reason some of the `Generic` classes of the `tuw_hardware_interface_template` have been replaced with `Dynamixel` classes with inheritance.

Note: this package is not ment to be used as standalone, it is rather intended to be used in another package.
For this reason the hardware interface is exported, with the following changes to the `package.xml` file:
```bash
  <export>
    <hardware_interface plugin="${prefix}/tuw_hardware_interface_trinamic.xml"/>
  </export>
```
Also check the `tuw_hardware_interface_trinamic.xml`.

## Application

This package contains all that is required to operate a joint with the Dynamixel MX-106.
This includes the following:
- a setup file: `resources/setup/trinamic_setup.yaml`
- a hardware description file:  `resources/hardware/TCMC-1640.yaml`
- a hardware config file:  `resources/config/TCMC-1640.yaml`
- a robot description (URDF) file: `resources/urdf/simple_trinamic_robot.yaml` which defines a single joint robot
- a controller definition: `resources/urdf/trinamic_setup.yaml`

These components are all required to start the `launch` file of this package with:
```bash
roslaunch tuw_iwos_hardware_interface_trinamic tuw_iwos_hardware_interface_trinamic_node.launch
```
This allows to start the hardware with the aforementioned components.