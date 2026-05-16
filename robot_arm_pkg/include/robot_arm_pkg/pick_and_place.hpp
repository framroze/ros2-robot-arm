#ifndef ROBOT_ARM_PKG__PICK_AND_PLACE_HPP_
#define ROBOT_ARM_PKG__PICK_AND_PLACE_HPP_

#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.hpp>
#include <moveit/planning_scene_interface/planning_scene_interface.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <string>

class PickAndPlace
{
public:
  explicit PickAndPlace(const rclcpp::Node::SharedPtr & node);

  // Run the full pick and place sequence
  void run();

private:
  rclcpp::Node::SharedPtr node_;

  // Move arm to a named target (e.g. "ready")
  void move_to_named(
    moveit::planning_interface::MoveGroupInterface & arm,
    const std::string & target_name,
    const std::string & label);

  // Plan and execute a pose target
  void move_to_pose(
    moveit::planning_interface::MoveGroupInterface & arm,
    const geometry_msgs::msg::Pose & pose,
    const std::string & label);

  // Execute a smooth Cartesian straight-line path
  void cartesian_move(
    moveit::planning_interface::MoveGroupInterface & arm,
    const geometry_msgs::msg::Pose & target,
    const std::string & label);

  // Helper to build a pose
  geometry_msgs::msg::Pose make_pose(
    double x, double y, double z,
    double qx = 1.0, double qy = 0.0,
    double qz = 0.0, double qw = 0.0);
};

#endif  // ROBOT_ARM_PKG__PICK_AND_PLACE_HPP_
