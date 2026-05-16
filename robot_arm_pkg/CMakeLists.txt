cmake_minimum_required(VERSION 3.8)
project(robot_arm_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(moveit_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(shape_msgs REQUIRED)

include_directories(include)

add_executable(arm_controller
  src/main.cpp
  src/pick_and_place.cpp
  src/scene_manager.cpp
)

ament_target_dependencies(arm_controller
  rclcpp
  std_msgs
  moveit_ros_planning_interface
  moveit_msgs
  geometry_msgs
  shape_msgs)

install(TARGETS arm_controller
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY include/
  DESTINATION include/)

ament_package()
