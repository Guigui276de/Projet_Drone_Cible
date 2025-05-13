#ifndef SERIAL_READER_HPP
#define SERIAL_READER_HPP

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <boost/asio.hpp>
#include <regex>
#include <string>
#include <sstream>
#include <chrono>

class SerialReader : public rclcpp::Node {
public:
    SerialReader();

private:
    void read_loop();
    std::string read_serial();
    std::string clean_data(const std::string &data);
    std::string hex_to_utf8(const std::string &hex);
    void extract_and_publish(const std::string &utf8_data);

    boost::asio::io_service io_service_;
    boost::asio::serial_port serial_;
    bool stopped_;
    std::chrono::steady_clock::time_point last_received_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr twist_publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

#endif // SERIAL_READER_HPP
