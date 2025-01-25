#include "rclcpp/rclcpp.hpp"
#include "ament_index_cpp/get_package_share_directory.hpp"
#include <fstream>
#include <sstream>
#include <string>

class URDFPublisher : public rclcpp::Node
{
public:
    URDFPublisher() : Node("urdf_publisher")
    {
        //std::string package_share_directory = ament_index_cpp::get_package_share_directory("kalman");
        std::string urdf_path = "/home/thomas/ros2_current_ws/src/hypo_core/kalman/src/robot.urdf";
        std::ifstream urdf_file(urdf_path);
        if (!urdf_file.is_open())
        {
            RCLCPP_ERROR(this->get_logger(), "Failed to open URDF file: %s", urdf_path.c_str());
            return;
        }

        std::stringstream urdf_string_stream;
        urdf_string_stream << urdf_file.rdbuf();
        std::string urdf_str = urdf_string_stream.str();
        
        this->declare_parameter("robot_description", urdf_str);
        this->set_parameter(rclcpp::Parameter("robot_description", urdf_str));

        RCLCPP_INFO(this->get_logger(), "URDF published successfully.");
    }
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<URDFPublisher>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
