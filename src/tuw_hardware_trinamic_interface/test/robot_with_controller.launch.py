#!/usr/bin/env python3

from launch import LaunchDescription
from launch_ros.actions import Node
from launch.substitutions import FileContent, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():
    # create a list to store the nodes in
    nodes = []

    # read the urdf file
    urdf_file = FileContent(
        PathJoinSubstitution([
            FindPackageShare('tuw_hardware_trinamic_interface'),
            'test',
            'test.urdf.xml'
        ])
    )

    # get the controller parameter file
    yaml = PathJoinSubstitution([
        FindPackageShare('tuw_hardware_trinamic_interface'),
        'test',
        'controller.yaml'
    ])

    # create the nodes
    # create the robot_state_publisher node
    robot_state = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='both',
        parameters=[{'robot_description': urdf_file}]
    )
    nodes.append(robot_state)

    # create the controller manager node
    controller_manager = Node(
        package='controller_manager',
        executable='ros2_control_node',
        name='controller_manager', # if it isn't called 'controller_manager', the whole system won"t work!
        output='both',
        parameters=[yaml]
    )
    nodes.append(controller_manager)

    # spawn the joint state broadcaster
    spawn_jsb = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['joint_state_broadcaster']
    )
    nodes.append(spawn_jsb)

    # spawn the test joint's controller
    spawn_test_joint_controller = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['test_joint_controller',
                   '--param-file',
                   yaml]
    )
    nodes.append(spawn_test_joint_controller)

    # return a LaunchDescription with the nodes
    return LaunchDescription(nodes)
