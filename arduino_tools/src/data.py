#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from std_msgs.msg import String, Int32, Float32, Float32MultiArray
import serial_comm
import numpy as np  

port = '/dev/ttyUSB0'
baudrate = 9600
port_serie = serial_comm.PortSerie(port, 9600)
port_serie.open_port()

# Define calibration parameters (magnetometers)
m_calib_matrix = np.array([[1.986469, 0.096475, 0.436912],
                           [0.096475, 2.107059, -0.050952],
                           [0.436912, -0.050952, 2.904628]])

m_calib_bias = np.array([-311.182950, -161.250873, -993.690982])

# Define calibration parameters (accelerometer)
a_calib_matrix = np.array([[1.003721, -0.001006, 0.001816],
                           [-0.001006, 0.993228, 0.000476],
                           [0.001816, 0.000476, 0.986258]])

a_calib_bias = np.array([0.028789, -0.021678, 0.065379])


class MultiPublisher(Node):

    def __init__(self):
        super().__init__('multi_publisher')
        self.m0_publisher = self.create_publisher(Float32MultiArray, 'm0_topic',10)
        
        # Create the magnetometer publisher
        self.m_publisher = self.create_publisher(Float32MultiArray, 'm_topic', 10)
        
        
        # Create the gyrometer publisher
        self.g_publisher = self.create_publisher(Float32MultiArray, 'g_topic', 10)

        # Create the accelerometer publishera
        self.a_publisher = self.create_publisher(Float32MultiArray, 'a_topic', 10)

        self.dt_publisher = self.create_publisher(Float32, 'dt_topic', 10)
        
        # Create timers for each publisher
        self.timer = self.create_timer(0.1, self.callback)

        self.m_msg = Float32MultiArray()
        self.g_msg = Float32MultiArray()
        self.a_msg = Float32MultiArray()  
        self.m0_msg = Float32MultiArray()   
        self.dt_msg = Float32()

    def callback(self):
        data_list = []
        data = port_serie.lire_donnees()
        data_list.append(data.split(','))
        if len(data_list[0]) ==13:
        
         m_raw_values =  np.array([float(data_list[0][0]), float(data_list[0][1]), float(data_list[0][2])])
         a_raw_values =  np.array([float(data_list[0][6]), float(data_list[0][7]), float(data_list[0][8])])
         m_raw_values0 = np.array([float(data_list[0][10]), float(data_list[0][11]), float(data_list[0][12])])

        
         m_calib_values = np.dot(m_calib_matrix,(m_raw_values - m_calib_bias))
         a_calib_values = np.dot(a_calib_matrix,(a_raw_values - a_calib_bias))
         m_calib_values0 = np.dot(m_calib_matrix,(m_raw_values0 - m_calib_bias))

         print(a_calib_values)


         self.m_msg.data = [m_calib_values[0],m_calib_values[1],m_calib_values[2]]
         self.g_msg.data = [float(data_list[0][3]),float(data_list[0][4]),float(data_list[0][5])]
         self.a_msg.data = [a_calib_values[0],a_calib_values[1],a_calib_values[2]]
         self.m0_msg.data = [m_calib_values0[0],m_calib_values0[1],m_calib_values0[2]]
         self.dt_msg.data = float(data_list[0][9])
        
        
         self.m_publisher.publish(self.m_msg)
         self.g_publisher.publish(self.g_msg)
         self.a_publisher.publish(self.a_msg)
         self.m0_publisher.publish(self.m0_msg)
         self.dt_publisher.publish(self.dt_msg)
         self.get_logger().info(f'Publishing to chatter:')

def main(args=None):
    rclpy.init(args=args)
    node = MultiPublisher()
    
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass

    # Destroy the node explicitly
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

