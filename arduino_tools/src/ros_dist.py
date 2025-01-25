#!/usr/bin/env python

import rclpy
import serial_comm
from rclpy.node import Node 
from std_msgs.msg import String
from std_msgs.msg import Float32


# Spécifiez le port série à utiliser, par exemple '/dev/ttyUSB0' sur Linux ou 'COM1' sur Windows
port = '/dev/ttyUSB0'  # Modifier selon votre configuration

baudrate = 9600

class MyNode(Node):

    def __init__(self):
        super().__init__("first_node")
        while True:
            serial_comm.lire_port_serie(port,baudrate)
            self.get_logger().info("Hello from ROS2")


def main(args=None):
    rclpy.init(args=args)
    node = MyNode()
    rclpy.spin(node)
    rclpy.shutdown()
    

if __name__ == '__main__':
    main()
