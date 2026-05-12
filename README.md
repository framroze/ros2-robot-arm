# ROS2 Robot Arm - Pick and Place

A ROS2 + C++ implementation of autonomous pick and place using the Franka Panda robot arm and MoveIt2 motion planning.

## Demo

The robot arm autonomously:
1. Sets up a realistic workspace (2 black tables, blue box, wooden floor, wall, vase)
2. Opens gripper and approaches the blue box on Table 1
3. Lowers, grasps the box with the gripper
4. Lifts and transports the box to Table 2
5. Places the box gently and releases
6. Returns to home position

## Tech Stack

- **ROS2 Jazzy** (Ubuntu 24.04)
- **C++17** with rclcpp
- **MoveIt2** for motion planning
- **RViz2** for visualization
- **Franka Panda** 7-DOF robot arm

## Requirements

- Ubuntu 24.04
- ROS2 Jazzy installed
- MoveIt2 installed

## Installation

```bash
# Install ROS2 Jazzy if not already installed
# See: https://docs.ros.org/en/jazzy/Installation.html

# Install required packages
sudo apt install -y \
  ros-jazzy-moveit \
  ros-jazzy-moveit-resources \
  ros-jazzy-rviz2 \
  python3-colcon-common-extensions

# Clone this repo into your ROS2 workspace src folder
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src
git clone https://github.com/framroze/ros2-robot-arm.git
cp -r ros2-robot-arm/robot_arm_pkg .

# Build
cd ~/ros2_ws
source /opt/ros/jazzy/setup.bash
colcon build --packages-select robot_arm_pkg
```

## How to Run

Open two terminals.

**Terminal 1** (launch MoveIt2 with Panda arm):
```bash
source /opt/ros/jazzy/setup.bash
ros2 launch moveit_resources_panda_moveit_config demo.launch.py
```

**Terminal 2** (run pick and place):
```bash
cd ~/ros2_ws
source install/setup.bash
ros2 run robot_arm_pkg arm_controller
```

## Project Structure
ros2-robot-arm/
├── robot_arm_pkg/
│   ├── src/
│   │   └── arm_controller.cpp    # Main pick and place logic
│   ├── CMakeLists.txt
│   └── package.xml
├── PROGRESS.md                   # Development progress notes
└── README.md

## Concepts Demonstrated

- ROS2 nodes, topics, publishers, subscribers
- C++ node structure with rclcpp
- MoveIt2 motion planning interface
- Planning scene with collision objects
- Gripper control (open/close)
- Object attach/detach for manipulation
- Cartesian path planning for smooth motion
- Pick and place programming in C++

## Author

framroze - [GitHub](https://github.com/framroze)

## License

Open source for educational use.
