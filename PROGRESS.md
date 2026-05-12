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
- Franka description: ~/ros2_ws/src/franka_description

## IMPORTANT - Build Fix (Miniconda conflict)
Always run these before building:
- rm -rf build/robot_arm_pkg (only if build fails)
- export PATH=/usr/bin:/usr/local/bin:/bin:/usr/sbin:/sbin
- export LD_LIBRARY_PATH=/opt/ros/jazzy/lib:/usr/lib/x86_64-linux-gnu
- export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:$LIBRARY_PATH
- source /opt/ros/jazzy/setup.bash

## Commands to Run Project
Terminal 1 (MoveIt2 demo):
- source /opt/ros/jazzy/setup.bash
- ros2 launch moveit_resources_panda_moveit_config demo.launch.py

Terminal 2 (Pick and Place):
- cd ~/ros2_ws
- source install/setup.bash
- ros2 run robot_arm_pkg arm_controller

## Project 1 - Robot Arm - COMPLETE ✅
- Franka FR3 real robot arm visualized in RViz2 ✅
- MoveIt2 motion planning working ✅
- C++ node controlling arm programmatically ✅
- Full pick and place sequence (6 steps) ✅
- All code pushed to GitHub ✅

## Concepts Learned
- ROS2 nodes, topics, publishers
- C++ node structure with rclcpp
- Building with colcon, CMakeLists.txt
- URDF robot description format
- RViz2 visualization
- TF transforms
- MoveIt2 motion planning
- Pick and place programming in C++

## Current Status
- PROJECT 1 COMPLETE ✅
- NEXT: Start Project 2 - Mobile Robot with LiDAR, Camera, SLAM, Navigation
