#!/usr/bin/env python3

from launch import LaunchDescription
from launch_ros.actions import Node
from launch.substitutions import FileContent, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():
    urdf_file = FileContent(
        PathJoinSubstitution([
            FindPackageShare('test_tuw_hardware_two_joints'),
            'urdf',
            'two_joints.urdf.xml'
        ])
    )

    yaml = PathJoinSubstitution([
        FindPackageShare('test_tuw_hardware_two_joints'),
        'bringup',
        'test_trinamic.no_controller.yaml'
    ])


    robot_state = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='both',
        parameters=[{'robot_description': urdf_file}]
    )

    controller_manager = Node(
        package='controller_manager',
        executable='ros2_control_node',
        name='controller_manager',
        output='both',
        parameters=[yaml]
    )

    spawn_jsb = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['joint_state_broadcaster']
    )

    spawn_dd = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['diff_drive_controller',
                   '--param-file',
                   yaml,
                   '--controller-ros-args',
                   '-r /diff_drive_controller/cmd_vel:=cmd_vel'
                   ]
    )

    return LaunchDescription([robot_state, controller_manager, spawn_jsb, spawn_dd])
