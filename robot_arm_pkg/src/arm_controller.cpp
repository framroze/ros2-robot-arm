#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.hpp>
#include <geometry_msgs/msg/pose.hpp>

class ArmController : public rclcpp::Node
{
public:
  ArmController() : Node("arm_controller", rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true))
  {
    RCLCPP_INFO(this->get_logger(), "Arm Controller Node Started!");
  }

  void move_to_pose()
  {
    // Create MoveIt2 interface for the panda arm
    auto move_group = moveit::planning_interface::MoveGroupInterface(shared_from_this(), "panda_arm");

    // Set target pose
    geometry_msgs::msg::Pose target_pose;
    target_pose.orientation.w = 1.0;
    target_pose.position.x = 0.3;
    target_pose.position.y = 0.0;
    target_pose.position.z = 0.5;

    move_group.setPoseTarget(target_pose);

    RCLCPP_INFO(this->get_logger(), "Planning motion...");

    // Plan
    moveit::planning_interface::MoveGroupInterface::Plan plan;
    bool success = (move_group.plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);

    if (success) {
      RCLCPP_INFO(this->get_logger(), "Plan successful! Executing...");
      move_group.execute(plan);
    } else {
      RCLCPP_ERROR(this->get_logger(), "Planning failed!");
    }
  }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<ArmController>();
  node->move_to_pose();
  rclcpp::shutdown();
  return 0;
}
