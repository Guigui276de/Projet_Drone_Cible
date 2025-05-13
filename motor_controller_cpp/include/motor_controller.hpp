#ifndef MOTOR_CONTROLLER_HPP
#define MOTOR_CONTROLLER_HPP

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <boost/asio.hpp>
#include <memory>

class MotorController : public rclcpp::Node {
public:
    MotorController();
    ~MotorController();

private:
    void cmdVelCallback(const geometry_msgs::msg::Twist::SharedPtr msg);
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription_;
    
    boost::asio::io_service io_service_;
    std::unique_ptr<boost::asio::serial_port> serial_;
};

#endif // MOTOR_CONTROLLER_HPP
