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
    )
    nodes.append(controller_manager)

    # return a LaunchDescription with the nodes
    return LaunchDescription(nodes)
