#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
#include <cmath>
#include "maths.h"

class MultipleSubscribers : public rclcpp::Node
{
public:
    MultipleSubscribers()
    : Node("multiple_subscribers_node")
    {
        
        /*MovingAverage mva_x;
        MovingAverage mva_y;
        MovingAverage mva_z;
        mva_x.stop_inf_nan = 5; mva_x.size_average = 10;
        mva_y.stop_inf_nan = 5; mva_y.size_average = 10;
        mva_z.stop_inf_nan = 5; mva_z.size_average = 10;*/
       
        // Création des subscribers
        m0_subscriber_ = this->create_subscription<std_msgs::msg::Float32MultiArray>(
            "m0_topic", 10,
            std::bind(&MultipleSubscribers::m0_callback, this, std::placeholders::_1));

        m_subscriber_ = this->create_subscription<std_msgs::msg::Float32MultiArray>(
            "m_topic", 10,
            std::bind(&MultipleSubscribers::m_callback, this, std::placeholders::_1));

        g_subscriber_ = this->create_subscription<std_msgs::msg::Float32MultiArray>(
            "g_topic", 10,
            std::bind(&MultipleSubscribers::g_callback, this, std::placeholders::_1));
        
        a_subscriber_ = this->create_subscription<std_msgs::msg::Float32MultiArray>(
            "a_topic", 10,
            std::bind(&MultipleSubscribers::a_callback, this, std::placeholders::_1));

        dt_subscriber_ = this->create_subscription<std_msgs::msg::Float32>(
            "dt_topic", 10,
            std::bind(&MultipleSubscribers::dt_callback, this, std::placeholders::_1));

        kalman_publisher_ = this->create_publisher<std_msgs::msg::Float32MultiArray>(
            "kalman_topic", 10);

        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&MultipleSubscribers::timer_callback, this));
    }

private:
    float _declination;
    float _pitch;
    float _roll;
    float _yaw;
    float _heading;
    float _mx, _my, _mz;
    float _gx, _gy, _gz;
    float _dt;
    float _Bx,_By;
    
    std_msgs::msg::Float32MultiArray kalman_msg;

    void m_callback(const std_msgs::msg::Float32MultiArray::SharedPtr msg)
    {
        _mx = msg->data[0];
        _my = msg->data[1];
        _mz = msg->data[2];
    }

    void g_callback(const std_msgs::msg::Float32MultiArray::SharedPtr msg)
    {
        _gx = (msg->data[0]*M_PI)/180;
        _gy = (msg->data[1]*M_PI)/180;
        _gz = (msg->data[2]*M_PI)/180;
    }

    void a_callback(const std_msgs::msg::Float32MultiArray::SharedPtr msg)
    {
        _pitch = atan2(-msg->data[0], sqrt(pow(msg->data[1], 2) + pow(msg->data[2], 2)));
        _roll = atan2(msg->data[1], msg->data[2]);
        //RCLCPP_INFO(this->get_logger(), "roll: %f", _roll);
        //RCLCPP_INFO(this->get_logger(), "pitch: %f", _pitch);
    }
    
    void m0_callback(const std_msgs::msg::Float32MultiArray::SharedPtr msg)
    {   if(msg->data[0] != 0)_declination = atan(msg->data[1]/msg->data[0]);
        if(msg->data[0] >=0 and msg->data[1] <= 0){
           _declination = M_PI + _declination;
        }  
        else if(msg->data[0] >= 0 and msg->data[1]>= 0){
           _declination = _declination - M_PI;
        }
        
        //RCLCPP_INFO(this->get_logger(), "declination: %f", _declination);
    }

    void timer_callback()
    {   _By = _my*cos(_roll) - _mz*sin(_roll);
        _Bx = _mx*cos(_pitch) + _my*sin(_pitch) + _mz*sin(_pitch)*cos(_roll);
        if(_mx != 0)_heading = atan(_my/_mx);
        if(_mx >=0 and _my <= 0){
           _heading = M_PI + _heading;
        }  
        else if(_mx >= 0 and _my >= 0){
           _heading = _heading - M_PI;
        }
        RCLCPP_INFO(this->get_logger(), "heading: %f", _heading);
        if (_declination <= 0 and _heading>=0)_yaw = 2*M_PI + _declination - _heading;
        else if (_declination >=0 and _heading <=0)_yaw = 2*M_PI - (_declination - _heading);
        else{_yaw =_declination - _heading;}
        RCLCPP_INFO(this->get_logger(), "yaw: %f", _yaw);
        kalman_msg.data = {_yaw, _pitch, _roll, _gx, _gy, _gz, _dt};
        kalman_publisher_->publish(kalman_msg);
        //RCLCPP_INFO(this->get_logger(), "Timer callback executed");
    }   

    void dt_callback(const std_msgs::msg::Float32::SharedPtr msg)
    {
        _dt = msg->data;
    }

    // Déclaration des subscribers et publisher
    rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr m0_subscriber_, a_subscriber_, m_subscriber_, g_subscriber_;
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr dt_subscriber_;
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr kalman_publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MultipleSubscribers>());
    rclcpp::shutdown();
    return 0;
}


