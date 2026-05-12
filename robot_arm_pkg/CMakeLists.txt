cmake_minimum_required(VERSION 3.8)
project(robot_arm_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(geometry_msgs REQUIRED)

add_executable(arm_controller src/arm_controller.cpp)
ament_target_dependencies(arm_controller 
  rclcpp 
  std_msgs
  moveit_ros_planning_interface
  geometry_msgs)

install(TARGETS arm_controller
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY urdf launch
  DESTINATION share/${PROJECT_NAME})

ament_package()
