cmake_minimum_required(VERSION 3.8)
project(com_manette_cpp)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(geometry_msgs REQUIRED)

add_executable(serial_reader_node2
    src/serial_reader2.cpp
    src/main.cpp
)

# Ajouter les dépendances dans `ament_target_dependencies`
ament_target_dependencies(serial_reader_node2
    rclcpp
    geometry_msgs
    Boost
)

# Inclure les répertoires d'en-têtes
target_include_directories(serial_reader_node2 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS serial_reader_node2
    DESTINATION lib/${PROJECT_NAME}
)

ament_package()
