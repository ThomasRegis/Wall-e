#include "maths2.h"

#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include "pcl_conversions/pcl_conversions.h"
#include "std_msgs/msg/float32_multi_array.hpp"
#include "std_msgs/msg/float32.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"

#include "pcl_plane_extractor2.h"        

class ZedPlaneNode : public rclcpp::Node {
public:
    ZedPlaneNode() : Node("zed_plane_node2"){
       // Récupération des paramètres
        this->get_parameter_or("~cloud_in", cloud_in_, std::string("default_cloud_in"));
        this->get_parameter_or("~dist_topic", dist_topic_, std::string("default_dist_topic"));
        this->get_parameter_or("~angle_topic", angle_topic_, std::string("default_angle_topic"));
        this->get_parameter_or("~distance_offset", distance_offset_, 0.0);
        this->get_parameter_or("~size_vector_average", size_average_, 10);
        this->get_parameter_or("~number_unvalid_stop", stop_inf_, 5);
        this->get_parameter_or("~use_moving_average", use_moving_average_, true);
        this->get_parameter_or("~eps_deg", eps_deg_, 0.5);
        this->get_parameter_or("~dist_thr", dist_thr_, 1.0);
        this->get_parameter_or("~max_iter", max_iter_, 100);
        this->get_parameter_or("~sick_gap", sick_gap_, 0.2);
        this->get_parameter_or("~type_angle", type_angle_, std::string("default_type_angle"));
        this->get_parameter_or("~filter", filter_, true);

     // Initialisation du subscriber
        cloud_in_sub_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
            "cloud_in", 10,
            std::bind(&ZedPlaneNode::zedPlaneCallback, this, std::placeholders::_1));

        // Initialisation des publishers
        plane_pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>(
            "/zed/plane", 10);
        coeff_pub_ = this->create_publisher<std_msgs::msg::Float32MultiArray>(
            "/zed/plane_coeff", 10);
        dist_pub_ = this->create_publisher<std_msgs::msg::Float32>(
            "distance_topic", 10);
        angle_pub_ = this->create_publisher<std_msgs::msg::Float32>(
            "angle_topic", 10);

        _average_dist.stop_inf_nan = _stop_inf; _average_angle.stop_inf_nan = _stop_inf;
    _   _average_dist.size_average = _size_average ; _average_angle.size_average = _size_average;
    }

private:
   
    std::string cloud_in_;
    std::string dist_topic_;
    std::string angle_topic_;
    double distance_offset_;
    int size_average_;
    int stop_inf_;
    bool use_moving_average_;
    double eps_deg_;
    double dist_thr_;
    int max_iter_;
    double sick_gap_;
    std::string type_angle_;
    bool filter_;

    // Exemple d'utilisation des messages déclarés
    std_msgs::msg::Float32 dist_msg;
    std_msgs::msg::Float32 angle_msg;
    std_msgs::msg::Float32MultiArray coeff_msg;
    sensor_msgs::msg::PointCloud2 plane_msg;

    float dist,angle,angle_radian,angle_tan,angle_dist;

    MovingAverage _average_dist;
    MovingAverage _average_angle;

    float coeff_a,coeff_b,coeff_c,coeff_d;
    std::vector<float> coeff_vector;


    void zedPlaneCallback(const sensor_msgs::msg::PointCloud2::SharedPtr cloud_msg) {
        // Extraction du plan à l'aide de PCL (à adapter selon votre méthode spécifique)
        PclPlaneExtractor zed_plane_extractor(cloud_msg, _max_iter, _eps_deg, _dist_thr, _filter);

        // Extraction des coefficients du plan
        float coeff_a = zed_plane_extractor.coeff0;
        float coeff_b = zed_plane_extractor.coeff1;
        float coeff_c = zed_plane_extractor.coeff2;
        float coeff_d = zed_plane_extractor.coeff3;

        // Calcul de l'angle en fonction du type spécifié
        float angle_tan = (coeff_b / coeff_a);
        float angle_dist = angle_tan * _sick_gap;
        float angle_radian = std::atan(angle_tan);
        float angle = 0.0;  // Variable pour stocker l'angle calculé

        if (_type_angle == "distance") {
            angle = angle_dist;
            RCLCPP_INFO(this->get_logger(), "angle_dist: %f", angle);
        } else if (_type_angle == "radian") {
            angle = angle_radian;
            RCLCPP_INFO(this->get_logger(), "angle_radian: %f", angle);
        } else if (_type_angle == "tan") {
            angle = angle_tan;
            RCLCPP_INFO(this->get_logger(), "angle_tan: %f", angle);
        } else {
            RCLCPP_WARN(this->get_logger(), "Le type d'angle sélectionné n'existe pas");
        }

        // Calcul de la distance
        float dist = std::abs(coeff_d / coeff_a) + _distance_offset;
        RCLCPP_INFO(this->get_logger(), "dist: %f", dist);

        // Utilisation de la moyenne mobile si activée
        if (_use_moving_average) {
            _average_dist.manageInfNanAndMovingAverage(dist);
            dist = _average_dist.moving_average;

            _average_angle.manageInfNanAndMovingAverage(angle);
            angle = _average_angle.moving_average;

            RCLCPP_INFO(this->get_logger(), "moyenne_angle : %f", angle);
            RCLCPP_INFO(this->get_logger(), "moyenne_dist : %f", dist);
        }

        // Publication des résultats
        std_msgs::msg::Float32MultiArray coeff_msg;
        coeff_msg.data = {coeff_a, coeff_b, coeff_c, coeff_d};
        coeff_pub_->publish(coeff_msg);

        std_msgs::msg::Float32 dist_msg;
        dist_msg.data = dist;
        dist_pub_->publish(dist_msg);

        std_msgs::msg::Float32 angle_msg;
        angle_msg.data = angle;
        angle_pub_->publish(angle_msg);

        sensor_msgs::msg::PointCloud2 plane_msg;
        pcl::toROSMsg(zed_plane_extractor.new_cloud, plane_msg);
        plane_pub_->publish(plane_msg);
    }
}

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ZedPlaneNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
