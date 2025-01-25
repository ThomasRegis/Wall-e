#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from std_msgs.msg import String, Int32, Float32, Float32MultiArray
import kalman_filter
from geometry_msgs.msg import TransformStamped
from tf2_ros import TransformBroadcaster
import tf_transformations
import math

kalman = kalman_filter.Kalman(0,0,0,1,1)
t = TransformStamped()


class Main(Node):

    def __init__(self):
        
        super().__init__('main')
        self.subscriber = self.create_subscription(Float32MultiArray, 'kalman_topic',self.callback,10)
        self.tf_broadcaster = TransformBroadcaster(self)

    def callback(self,msg):
        kalman.prediction(msg.data[5],msg.data[4],msg.data[3],msg.data[6])
        kalman.update(msg.data[0],msg.data[1],msg.data[2])
        print(kalman.state)
        

        # Configurer les informations de l'en-tête
        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = 'world'
        t.child_frame_id = 'chassis'

        quaternion = tf_transformations.quaternion_from_euler(kalman.state[2][0],kalman.state[1][0] ,kalman.state[0][0])

        # Exemple de transformation dynamique (une rotation simple)
        t.transform.translation.x = 0.0
        t.transform.translation.y = 1.0
        t.transform.translation.z = 0.0
        
        t.transform.rotation.x = quaternion[0]
        t.transform.rotation.y = quaternion[1]
        t.transform.rotation.z = 0.0 #quaternion[2]
        t.transform.rotation.w = quaternion[3]
        

        # Envoyer la transformation
        self.tf_broadcaster.sendTransform(t)


        
def main(args=None):
    rclpy.init(args=args)
    node = Main()
    
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass

    # Destroy the node explicitly
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()