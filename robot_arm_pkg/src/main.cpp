#include <rclcpp/rclcpp.hpp>
#include "robot_arm_pkg/pick_and_place.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<rclcpp::Node>(
    "arm_controller",
    rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true));

  RCLCPP_INFO(node->get_logger(), "========================================");
  RCLCPP_INFO(node->get_logger(), "   ROS2 Robot Arm — Pick and Place      ");
  RCLCPP_INFO(node->get_logger(), "   Franka Panda + MoveIt2 + C++        ");
  RCLCPP_INFO(node->get_logger(), "========================================");

  PickAndPlace pick_and_place(node);
  pick_and_place.run();

  rclcpp::shutdown();
  return 0;
}
