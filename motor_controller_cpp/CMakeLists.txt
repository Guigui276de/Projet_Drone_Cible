cmake_minimum_required(VERSION 3.8)
project(motor_controller_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
#Ajout des dépendances
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)

find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})
#Inclusion du dossier include pour le .hpp
include_directories(include)

add_executable(motor_controller_node
    src/motor_controller.cpp
    src/motor_controller_node.cpp
)

target_link_libraries(motor_controller_node ${Boost_LIBRARIES})
ament_target_dependencies(motor_controller_node rclcpp geometry_msgs)


#Procedure d'installation
install(TARGETS motor_controller_node DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
