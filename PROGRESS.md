# 📝 Development Progress

## 🖥️ System Info
- **GitHub:** [framroze](https://github.com/framroze)
- **OS:** Ubuntu 24.04 LTS
- **ROS2:** Jazzy

## 📂 Workspace
- ROS2 workspace: `~/ros2_ws`
- Robot arm package: `~/ros2_ws/src/robot_arm_pkg`
- Franka description: `~/ros2_ws/src/franka_description`

---

## ⚠️ Important — Build Fix (Miniconda Conflict)

If `colcon build` fails due to library conflicts with Miniconda, run these commands before building:

```bash
rm -rf build/robot_arm_pkg
export PATH=/usr/bin:/usr/local/bin:/bin:/usr/sbin:/sbin
export LD_LIBRARY_PATH=/opt/ros/jazzy/lib:/usr/lib/x86_64-linux-gnu
export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:$LIBRARY_PATH
source /opt/ros/jazzy/setup.bash
```

---

## ▶️ How to Run

**Terminal 1** — Launch MoveIt2:
```bash
source /opt/ros/jazzy/setup.bash
ros2 launch moveit_resources_panda_moveit_config demo.launch.py
```

**Terminal 2** — Run the pick and place:
```bash
cd ~/ros2_ws
source install/setup.bash
ros2 run robot_arm_pkg arm_controller
```

---

## ✅ Completed Milestones

- [x] Franka Panda professional robot arm visualized in RViz2
- [x] MoveIt2 motion planning fully working
- [x] C++ code controls the arm programmatically
- [x] Realistic workspace built: wooden floor, back wall, 2 black tables, blue box, red vase
- [x] Full pick and place — gripper opens, lowers, grasps the box
- [x] Lifts and carries box from **Table 1** to **Table 2**
- [x] Places box gently and releases
- [x] Returns to home position
- [x] Cartesian path planning for smooth motion
- [x] All code pushed to GitHub

---

## 🧠 Concepts Learned

- ROS2 nodes, topics, publishers, subscribers
- C++ node structure with `rclcpp`
- Building with `colcon` and `CMakeLists.txt`
- URDF robot description format
- RViz2 visualization
- TF transforms
- MoveIt2 motion planning
- Planning scene with collision objects
- Gripper control (open / close)
- Object attach / detach during manipulation
- Cartesian path planning for smooth motion
- Pick and place programming in C++

---

## 🚦 Status

🟢 **PROJECT COMPLETE**
