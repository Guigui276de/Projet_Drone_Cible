#include "SerialReader.hpp"

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SerialReader>();  // <-- Appelle SerialReader()
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
