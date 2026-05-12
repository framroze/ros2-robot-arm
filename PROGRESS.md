# ROS2 Robot Arm - Development Notes

## System Info
- GitHub: https://github.com/framroze
- Ubuntu 24.04, Miniconda installed

## Repos
- ~/ros2-robot-arm → git@github.com:framroze/ros2-robot-arm.git
- ~/ros2-mobile-robot → git@github.com:framroze/ros2-mobile-robot.git

## Workspace
- ROS2 workspace: ~/ros2_ws
- Robot arm package: ~/ros2_ws/src/robot_arm_pkg
- Franka description: ~/ros2_ws/src/franka_description

## IMPORTANT - Build Fix (Miniconda conflict)
Before building, ALWAYS run:
- rm -rf build/robot_arm_pkg (if build fails)
- export PATH=/usr/bin:/usr/local/bin:/bin:/usr/sbin:/sbin
- export LD_LIBRARY_PATH=/opt/ros/jazzy/lib:/usr/lib/x86_64-linux-gnu
- export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:$LIBRARY_PATH
- source /opt/ros/jazzy/setup.bash

## How to Run Project 1
Terminal 1:
- source /opt/ros/jazzy/setup.bash
- ros2 launch moveit_resources_panda_moveit_config demo.launch.py

Terminal 2:
- cd ~/ros2_ws
- source install/setup.bash
- ros2 run robot_arm_pkg arm_controller

## Project 1 - Robot Arm - COMPLETE ✅
- Franka Panda professional robot arm in RViz2 ✅
- MoveIt2 motion planning fully working ✅
- C++ code controls arm programmatically ✅
- Realistic workspace: wooden floor, back wall, 2 black tables, blue box, red vase ✅
- Full pick and place: gripper opens, lowers, grasps box ✅
- Lifts and carries box from Table 1 to Table 2 ✅
- Places box and releases gently ✅
- Returns to home position ✅
- Cartesian path planning for smooth motion ✅
- All code pushed to GitHub ✅

## Concepts Learned (Project 1)
- ROS2 nodes, topics, publishers, subscribers
- C++ node structure with rclcpp
- Building with colcon, CMakeLists.txt
- URDF robot description format
- RViz2 visualization
- TF transforms
- MoveIt2 motion planning
- Planning scene with collision objects
- Gripper control (open/close)
- Object attach/detach
- Cartesian path planning for smooth motion
- Pick and place programming in C++

## Current Status
- PROJECT 1 COMPLETE ✅
- NEXT: Project 2 - Mobile Robot with LiDAR, Camera, SLAM, Navigation
