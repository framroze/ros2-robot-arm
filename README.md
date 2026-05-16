# 🤖 ROS2 Robot Arm — Autonomous Pick and Place

<p align="center">
  <img src="https://img.shields.io/badge/ROS2-Jazzy-22314E?style=for-the-badge&logo=ros&logoColor=white" alt="ROS2"/>
  <img src="https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++"/>
  <img src="https://img.shields.io/badge/MoveIt2-Motion%20Planning-2BAF2B?style=for-the-badge" alt="MoveIt2"/>
  <img src="https://img.shields.io/badge/Ubuntu-24.04-E95420?style=for-the-badge&logo=ubuntu&logoColor=white" alt="Ubuntu"/>
  <img src="https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge" alt="License"/>
</p>

<p align="center">
  <b>A fully autonomous pick-and-place system built with ROS2 and C++, using the Franka Panda robot arm.</b>
</p>

---

## ✨ What it does

The robot arm autonomously performs a complete pick-and-place sequence in a realistic workspace:

1. 🛠️ Sets up the scene — wooden floor, back wall, two black tables, a blue package, and a decorative vase
2. 🤲 Opens the gripper and approaches the blue box on **Table 1**
3. ⬇️ Lowers smoothly using Cartesian path planning
4. ✊ Closes the gripper and grasps the box
5. ⬆️ Lifts and transports the box to **Table 2** (different height)
6. 📦 Places the box gently on Table 2 and releases
7. 🏠 Returns to home position

---

## 🧰 Tech Stack

| Component | Used For |
|-----------|----------|
| **ROS2 Jazzy** | Robotics middleware |
| **C++17 / rclcpp** | Robot control logic |
| **MoveIt2** | Motion planning & inverse kinematics |
| **RViz2** | 3D visualization |
| **Franka Panda** | 7-DOF robot arm model |
| **Cartesian Paths** | Smooth straight-line motion |

---

## 📋 Requirements

- Ubuntu 24.04 LTS
- ROS2 Jazzy installed ([Install Guide](https://docs.ros.org/en/jazzy/Installation.html))
- MoveIt2 for ROS2 Jazzy

---

## ⚙️ Installation

### 1️⃣ Install required packages

```bash
sudo apt update
sudo apt install -y \
  ros-jazzy-moveit \
  ros-jazzy-moveit-resources \
  ros-jazzy-rviz2 \
  python3-colcon-common-extensions \
  git
```

### 2️⃣ Clone this repository

```bash
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src
git clone https://github.com/framroze/ros2-robot-arm.git
cp -r ros2-robot-arm/robot_arm_pkg .
```

### 3️⃣ Clone the Franka robot model (required)

```bash
cd ~/ros2_ws/src
git clone https://github.com/frankaemika/franka_description.git
```

### 4️⃣ Build the package

```bash
cd ~/ros2_ws
source /opt/ros/jazzy/setup.bash
colcon build --packages-select robot_arm_pkg
```

---

## 🚀 How to Run

Open **two terminals**.

### 🖥️ Terminal 1 — Launch MoveIt2 with Panda Arm

```bash
source /opt/ros/jazzy/setup.bash
ros2 launch moveit_resources_panda_moveit_config demo.launch.py
```

> RViz2 will open showing the robot arm.

### 🖥️ Terminal 2 — Run the Pick and Place

```bash
cd ~/ros2_ws
source install/setup.bash
ros2 run robot_arm_pkg arm_controller
```

> Watch the arm autonomously perform pick and place in RViz2. 🎉

---

## 🩺 Troubleshooting

### Build fails with `libssl` or `libcrypto` errors (Miniconda/Anaconda users)

```bash
rm -rf build/robot_arm_pkg
export PATH=/usr/bin:/usr/local/bin:/bin:/usr/sbin:/sbin
export LD_LIBRARY_PATH=/opt/ros/jazzy/lib:/usr/lib/x86_64-linux-gnu
export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:$LIBRARY_PATH
source /opt/ros/jazzy/setup.bash
colcon build --packages-select robot_arm_pkg
```

### Python module errors (`catkin_pkg`, `empy`)

```bash
pip install catkin-pkg lark empy==3.3.4 --force-reinstall
```

---

## 📂 Project Structure

| Path | Description |
|------|-------------|
| 📦 [`robot_arm_pkg/`](robot_arm_pkg/) | ROS2 C++ package |
| 📄 [`robot_arm_pkg/src/arm_controller.cpp`](robot_arm_pkg/src/arm_controller.cpp) | Main pick and place logic |
| 📄 [`robot_arm_pkg/CMakeLists.txt`](robot_arm_pkg/CMakeLists.txt) | Build configuration |
| 📄 [`robot_arm_pkg/package.xml`](robot_arm_pkg/package.xml) | Package manifest |
| 📝 [`PROGRESS.md`](PROGRESS.md) | Development notes |
| 📖 [`README.md`](README.md) | This file |

---

## 🧠 Concepts Demonstrated

- ✅ ROS2 nodes, topics, publishers, subscribers
- ✅ C++ node structure using `rclcpp`
- ✅ MoveIt2 motion planning interface
- ✅ Planning scene with collision objects
- ✅ Gripper control (open / close)
- ✅ Object attach / detach during manipulation
- ✅ Cartesian path planning for smooth motion
- ✅ Pick-and-place programming in C++
- ✅ Building ROS2 packages with `colcon` and `CMakeLists.txt`

---

## 🎬 Highlights

- 🟢 **Real Franka Panda model** — Industry-grade 7-DOF robot arm
- 🟢 **Realistic workspace** — Wooden floor, wall, two black tables, vase
- 🟢 **Smooth Cartesian motion** — Straight-line paths for natural movement
- 🟢 **Full grasp control** — Gripper opens, closes, attaches, releases
- 🟢 **Clean modular C++** — Reusable helper functions for any pose target

---

## 👤 Author

**framroze**  
🌐 GitHub: [@framroze](https://github.com/framroze)

---

## 📜 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.
