# ROS2 Internship Progress

## Profile
- GitHub: https://github.com/framroze
- Ubuntu 24.04, Miniconda installed, MuJoCo experience
- Goal: internship with Talha (needs ROS2 + C++ projects)

## Repos
- ~/ros2-robot-arm → git@github.com:framroze/ros2-robot-arm.git
- ~/ros2-mobile-robot → git@github.com:framroze/ros2-mobile-robot.git

## Workspace
- ROS2 workspace: ~/ros2_ws
- Robot arm package: ~/ros2_ws/src/robot_arm_pkg
- Franka FR3 package: ~/ros2_ws/src/franka_description

## Setup Completed
- Git + SSH connected to GitHub ✅
- Both repos cloned to Ubuntu ✅
- ROS2 Jazzy installed and working ✅
- First C++ ROS2 node written and running ✅
- Gazebo Harmonic installed ✅
- Franka FR3 real robot arm visualized in RViz2 ✅
- All code pushed to GitHub ✅

## Commands to Resume
- cd ~/ros2_ws
- source /opt/ros/jazzy/setup.bash
- source install/setup.bash
- ros2 launch franka_description visualize_franka.launch.py robot_type:=fr3

## Concepts Learned
- ROS2 nodes, topics, publishers
- C++ node structure with rclcpp
- Building with colcon
- CMakeLists.txt for ROS2
- URDF robot description format
- RViz2 visualization
- TF transforms

## Current Status
- NEXT STEP: Install MoveIt2 and make the Franka arm do pick and place
