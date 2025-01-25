import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    # Chemin vers le fichier URDF du robot
    urdf_file_path = os.path.join(get_package_share_directory('kalman'), 'urdf', 'robot.urdf')

    # Lire le contenu du fichier URDF
    with open(urdf_file_path, 'r') as urdf_file:
        robot_description = urdf_file.read()

    # Noeud robot_state_publisher pour publier la description URDF
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[{'robot_description': robot_description}]
    )

    # Noeud pour lancer RViz2 avec une configuration
    rviz_config_file = os.path.join(get_package_share_directory('kalman'), 'rviz', 'robot_config.rviz')
    rviz2 = Node(
        package='rviz2',
        executable='rviz2',
        arguments=['-d', rviz_config_file],
        output='screen'
    )

    return LaunchDescription([
        robot_state_publisher,
        rviz2
    ])



