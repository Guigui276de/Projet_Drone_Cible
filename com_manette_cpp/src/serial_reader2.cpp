#include "SerialReader.hpp"
#include <boost/system/system_error.hpp>
#include <iostream>
#include <regex>

SerialReader::SerialReader()
    : Node("serial_reader"), serial_(io_service_), stopped_(false),
      last_received_(std::chrono::steady_clock::now()) {
    
    std::string port = "/dev/ttyUSB1";  // À adapter
    unsigned int baud_rate = 9600;

    try {
        serial_.open(port);
        serial_.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    } catch (boost::system::system_error &e) {
        RCLCPP_ERROR(this->get_logger(), "Erreur ouverture port: %s", e.what());
        return;
    }

    twist_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    timer_ = this->create_wall_timer(
        std::chrono::milliseconds(100), std::bind(&SerialReader::read_loop, this)
    );
}

void SerialReader::read_loop() {
    std::string data = read_serial();
    data = clean_data(data);
    if (!data.empty()) {
        RCLCPP_INFO(this->get_logger(), "Trame nettoyée: %s", data.c_str());

        std::string utf8_data = hex_to_utf8(data);
        RCLCPP_INFO(this->get_logger(), "Trame convertie UTF-8: %s", utf8_data.c_str());

        if (utf8_data.empty()) {
            RCLCPP_INFO(this->get_logger(), "Trame ignorée, vide après conversion");
            return;
        }

        last_received_ = std::chrono::steady_clock::now();

        if (utf8_data.find("dmx") != std::string::npos) {
            extract_and_publish(utf8_data);
        } else {
            RCLCPP_INFO(this->get_logger(), "Trame reçue mais ignorée (non manette)");
        }
    }

    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - last_received_;
    if (elapsed.count() > 5.0) {
        if (!stopped_) {
            stopped_ = true;
            auto twist_msg = geometry_msgs::msg::Twist();
            twist_publisher_->publish(twist_msg);
            RCLCPP_INFO(this->get_logger(), "Aucune trame reçue, publication de Twist arrêtée.");
        }
    } else {
        stopped_ = false;
    }
}

std::string SerialReader::read_serial() {
    boost::asio::streambuf buffer;
    boost::asio::read_until(serial_, buffer, "\n");
    std::istream is(&buffer);
    std::string data;
    std::getline(is, data);
    return data;
}

std::string SerialReader::clean_data(const std::string &data) {
    return std::regex_replace(data, std::regex("[ \n\r]+"), "");
}

std::string SerialReader::hex_to_utf8(const std::string &hex) {
    std::string result;
    std::regex pattern("Data:\\(HEX:\\)([0-9a-fA-F]+)");
    std::smatch match;
    if (std::regex_search(hex, match, pattern) && match.size() == 2) {
        std::string hex_values = match[1].str();
        for (size_t i = 0; i < hex_values.length(); i += 2) {
            std::string byte = hex_values.substr(i, 2);
            char chr = static_cast<char>(std::stoi(byte, nullptr, 16));
            result += chr;
        }
    }
    return result;
}

void SerialReader::extract_and_publish(const std::string &utf8_data) {
    if (stopped_) {
        RCLCPP_WARN(this->get_logger(), "Publication bloquée, trame ignorée : %s", utf8_data.c_str());
        return;
    }

    std::regex pattern("dmx(-?\\d+)y(-?\\d+)f");
    std::smatch match;
    if (std::regex_search(utf8_data, match, pattern) && match.size() == 3) {
        int x_value = std::stoi(match[1].str());
        int y_value = std::stoi(match[2].str());

        auto twist_msg = geometry_msgs::msg::Twist();
        twist_msg.linear.x = static_cast<double>(x_value) / 100;
        twist_msg.angular.z = -static_cast<double>(y_value) / 100;

        twist_publisher_->publish(twist_msg);
        RCLCPP_INFO(this->get_logger(), "Twist publié: linear.x=%.2f, angular.z=%.2f",
                    twist_msg.linear.x, twist_msg.angular.z);
    }
}
