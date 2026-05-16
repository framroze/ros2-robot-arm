#include "robot_arm_pkg/pick_and_place.hpp"
#include "robot_arm_pkg/scene_manager.hpp"

PickAndPlace::PickAndPlace(const rclcpp::Node::SharedPtr & node)
: node_(node)
{
}

void PickAndPlace::run()
{
  // Setup MoveIt2 interfaces
  auto arm = moveit::planning_interface::MoveGroupInterface(node_, "panda_arm");
  auto gripper = moveit::planning_interface::MoveGroupInterface(node_, "hand");
  moveit::planning_interface::PlanningSceneInterface planning_scene;

  // Motion settings - smooth and slow
  arm.setMaxVelocityScalingFactor(0.15);
  arm.setMaxAccelerationScalingFactor(0.15);
  arm.setPlanningTime(15.0);
  arm.setNumPlanningAttempts(20);
  arm.setGoalPositionTolerance(0.005);
  arm.setGoalOrientationTolerance(0.02);

  // Step 1: Build the scene
  RCLCPP_INFO(node_->get_logger(), "Step 1: Building workspace...");
  SceneManager scene_manager(node_->get_logger());
  scene_manager.setup_scene(planning_scene);
  rclcpp::sleep_for(std::chrono::seconds(2));

  // Step 2: Open gripper
  RCLCPP_INFO(node_->get_logger(), "Step 2: Opening gripper...");
  gripper.setNamedTarget("open");
  gripper.move();
  rclcpp::sleep_for(std::chrono::milliseconds(800));

  // Step 3: Move to home
  RCLCPP_INFO(node_->get_logger(), "Step 3: Moving to home position...");
  move_to_named(arm, "ready", "HOME");
  rclcpp::sleep_for(std::chrono::milliseconds(800));

  // Step 4: Move above the box
  RCLCPP_INFO(node_->get_logger(), "Step 4: Approaching box...");
  auto pre_grasp = make_pose(0.45, 0.0, 0.55);
  move_to_pose(arm, pre_grasp, "PRE-GRASP");
  rclcpp::sleep_for(std::chrono::milliseconds(800));

  // Step 5: Lower to grasp
  RCLCPP_INFO(node_->get_logger(), "Step 5: Lowering to grasp...");
  auto grasp_pose = make_pose(0.45, 0.0, 0.39);
  cartesian_move(arm, grasp_pose, "LOWER");
  rclcpp::sleep_for(std::chrono::milliseconds(600));

  // Step 6: Close gripper
  RCLCPP_INFO(node_->get_logger(), "Step 6: Grasping box...");
  gripper.setNamedTarget("close");
  gripper.move();
  RCLCPP_INFO(node_->get_logger(), "Box GRASPED!");
  rclcpp::sleep_for(std::chrono::milliseconds(1000));

  // Step 7: Attach box to gripper
  RCLCPP_INFO(node_->get_logger(), "Step 7: Attaching box...");
  arm.attachObject("box", "panda_link8", {"panda_leftfinger", "panda_rightfinger"});
  rclcpp::sleep_for(std::chrono::milliseconds(400));

  // Step 8: Lift box up
  RCLCPP_INFO(node_->get_logger(), "Step 8: Lifting box...");
  auto lift_pose = make_pose(0.45, 0.0, 0.65);
  cartesian_move(arm, lift_pose, "LIFT");
  rclcpp::sleep_for(std::chrono::milliseconds(800));

  // Step 9: Transport to Table 2
  RCLCPP_INFO(node_->get_logger(), "Step 9: Transporting to Table 2...");
  auto above_t2 = make_pose(0.0, 0.50, 0.65);
  move_to_pose(arm, above_t2, "TRANSPORT");
  rclcpp::sleep_for(std::chrono::milliseconds(800));

  // Step 10: Lower to place
  RCLCPP_INFO(node_->get_logger(), "Step 10: Placing box...");
  auto place_pose = make_pose(0.0, 0.50, 0.45);
  cartesian_move(arm, place_pose, "PLACE");
  rclcpp::sleep_for(std::chrono::milliseconds(600));

  // Step 11: Open gripper to release
  RCLCPP_INFO(node_->get_logger(), "Step 11: Releasing box...");
  gripper.setNamedTarget("open");
  gripper.move();
  RCLCPP_INFO(node_->get_logger(), "Box RELEASED on Table 2!");
  rclcpp::sleep_for(std::chrono::milliseconds(800));

  // Step 12: Detach box
  arm.detachObject("box");
  rclcpp::sleep_for(std::chrono::milliseconds(400));

  // Step 13: Retreat up
  RCLCPP_INFO(node_->get_logger(), "Step 12: Retreating...");
  auto retreat = make_pose(0.0, 0.50, 0.65);
  cartesian_move(arm, retreat, "RETREAT");
  rclcpp::sleep_for(std::chrono::milliseconds(800));

  // Step 14: Return home
  RCLCPP_INFO(node_->get_logger(), "Step 13: Returning home...");
  move_to_named(arm, "ready", "RETURN HOME");
  gripper.setNamedTarget("close");
  gripper.move();

  RCLCPP_INFO(node_->get_logger(), "=== Mission Complete! Box moved from Table 1 to Table 2! ===");
}

void PickAndPlace::move_to_named(
  moveit::planning_interface::MoveGroupInterface & arm,
  const std::string & target_name,
  const std::string & label)
{
  arm.setNamedTarget(target_name);
  moveit::planning_interface::MoveGroupInterface::Plan plan;
  bool success = (arm.plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);
  if (success) {
    RCLCPP_INFO(node_->get_logger(), "[%s] Executing...", label.c_str());
    arm.execute(plan);
    RCLCPP_INFO(node_->get_logger(), "[%s] Done.", label.c_str());
  } else {
    RCLCPP_ERROR(node_->get_logger(), "[%s] Planning failed!", label.c_str());
  }
}

void PickAndPlace::move_to_pose(
  moveit::planning_interface::MoveGroupInterface & arm,
  const geometry_msgs::msg::Pose & pose,
  const std::string & label)
{
  arm.setPoseTarget(pose);
  moveit::planning_interface::MoveGroupInterface::Plan plan;
  bool success = (arm.plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);
  if (success) {
    RCLCPP_INFO(node_->get_logger(), "[%s] Executing...", label.c_str());
    arm.execute(plan);
    RCLCPP_INFO(node_->get_logger(), "[%s] Done.", label.c_str());
  } else {
    RCLCPP_ERROR(node_->get_logger(), "[%s] Planning failed!", label.c_str());
  }
}

void PickAndPlace::cartesian_move(
  moveit::planning_interface::MoveGroupInterface & arm,
  const geometry_msgs::msg::Pose & target,
  const std::string & label)
{
  std::vector<geometry_msgs::msg::Pose> waypoints;
  waypoints.push_back(target);

  moveit_msgs::msg::RobotTrajectory traj;
  double fraction = arm.computeCartesianPath(waypoints, 0.003, traj);

  if (fraction > 0.7) {
    RCLCPP_INFO(node_->get_logger(), "[%s] Smooth path %.0f%%", label.c_str(), fraction * 100);
    arm.execute(traj);
    RCLCPP_INFO(node_->get_logger(), "[%s] Done.", label.c_str());
  } else {
    RCLCPP_WARN(node_->get_logger(), "[%s] Falling back to standard planning...", label.c_str());
    move_to_pose(arm, target, label);
  }
}

geometry_msgs::msg::Pose PickAndPlace::make_pose(
  double x, double y, double z,
  double qx, double qy, double qz, double qw)
{
  geometry_msgs::msg::Pose pose;
  pose.position.x = x;
  pose.position.y = y;
  pose.position.z = z;
  pose.orientation.x = qx;
  pose.orientation.y = qy;
  pose.orientation.z = qz;
  pose.orientation.w = qw;
  return pose;
}
