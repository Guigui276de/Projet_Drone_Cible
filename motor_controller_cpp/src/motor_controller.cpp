#include "motor_controller.hpp"
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>

MotorController::MotorController() : Node("motor_controller") {
    // Création de la souscription au topic cmd_vel
    subscription_ = this->create_subscription<geometry_msgs::msg::Twist>(
        "cmd_vel", 10, std::bind(&MotorController::cmdVelCallback, this, std::placeholders::_1));

    // Initialisation du port série
    try {
        serial_ = std::make_unique<boost::asio::serial_port>(io_service_, "/dev/ttyACM0");
        serial_->set_option(boost::asio::serial_port_base::baud_rate(115200));  // Définir le baudrate

        RCLCPP_INFO(this->get_logger(), "Port série ouvert à 115200 bauds.");
    } catch (const std::exception &e) {
        RCLCPP_ERROR(this->get_logger(), "Erreur ouverture port série : %s", e.what());
    }
}

MotorController::~MotorController() {
    if (serial_ && serial_->is_open()) {
        serial_->close();
    }
}

void MotorController::cmdVelCallback(const geometry_msgs::msg::Twist::SharedPtr msg) {
    int v1 = static_cast<int>((msg->linear.x - msg->angular.z) * 100);
    int v2 = static_cast<int>((msg->linear.x + msg->angular.z) * 100);

    // Limitation des vitesses pour éviter d'envoyer des valeurs trop grandes
    v1 = std::max(-255, std::min(255, v1));
    v2 = std::max(-255, std::min(255, v2));

    // Si le port série est ouvert, on envoie la commande
    if (serial_ && serial_->is_open()) {
        try {
            // Construction de la commande série
            std::string command = "V1:" + std::to_string(v1) + " V2:" + std::to_string(v2) + "\n";
            // Envoi de la commande via le port série
            boost::asio::write(*serial_, boost::asio::buffer(command));

            // Log de la commande envoyée
            RCLCPP_INFO(this->get_logger(), "Envoyé : %s", command.c_str());

            // Ajout d'un délai pour éviter de surcharger le port série
            std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Attendre 100ms avant d'envoyer la prochaine commande
        } catch (const boost::system::system_error& e) {
            // Gestion des erreurs d'écriture sur le port série
            RCLCPP_ERROR(this->get_logger(), "Erreur d'écriture sur le port série : %s", e.what());
        } catch (const std::exception &e) {
            // Gestion des autres exceptions
            RCLCPP_ERROR(this->get_logger(), "Erreur dans la commande série : %s", e.what());
        }
    } else {
        RCLCPP_ERROR(this->get_logger(), "Port série non ouvert ou non valide.");
    }
}
