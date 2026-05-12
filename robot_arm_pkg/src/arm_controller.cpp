#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.hpp>
#include <geometry_msgs/msg/pose.hpp>

class ArmController : public rclcpp::Node
{
public:
  ArmController() : Node("arm_controller", rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true))
  {
    RCLCPP_INFO(this->get_logger(), "=== Robot Arm Pick and Place Controller ===");
  }

  void run_pick_and_place()
  {
    auto move_group = moveit::planning_interface::MoveGroupInterface(shared_from_this(), "panda_arm");
    move_group.setMaxVelocityScalingFactor(0.3);
    move_group.setMaxAccelerationScalingFactor(0.3);

    // --- Step 1: Move to HOME position ---
    RCLCPP_INFO(this->get_logger(), "Step 1: Moving to HOME position...");
    move_group.setNamedTarget("ready");
    move_and_execute(move_group, "HOME");

    rclcpp::sleep_for(std::chrono::seconds(1));

    // --- Step 2: Move ABOVE the object ---
    RCLCPP_INFO(this->get_logger(), "Step 2: Moving ABOVE object...");
    geometry_msgs::msg::Pose above_object;
    above_object.orientation.w = 1.0;
    above_object.position.x = 0.4;
    above_object.position.y = 0.0;
    above_object.position.z = 0.5;
    move_group.setPoseTarget(above_object);
    move_and_execute(move_group, "ABOVE OBJECT");

    rclcpp::sleep_for(std::chrono::seconds(1));

    // --- Step 3: Move DOWN to pick ---
    RCLCPP_INFO(this->get_logger(), "Step 3: Moving DOWN to PICK object...");
    geometry_msgs::msg::Pose pick_pose;
    pick_pose.orientation.w = 1.0;
    pick_pose.position.x = 0.4;
    pick_pose.position.y = 0.0;
    pick_pose.position.z = 0.3;
    move_group.setPoseTarget(pick_pose);
    move_and_execute(move_group, "PICK");

    rclcpp::sleep_for(std::chrono::seconds(1));

    // --- Step 4: Move UP with object ---
    RCLCPP_INFO(this->get_logger(), "Step 4: Moving UP with object...");
    move_group.setPoseTarget(above_object);
    move_and_execute(move_group, "LIFT");

    rclcpp::sleep_for(std::chrono::seconds(1));

    // --- Step 5: Move to PLACE position ---
    RCLCPP_INFO(this->get_logger(), "Step 5: Moving to PLACE position...");
    geometry_msgs::msg::Pose place_pose;
    place_pose.orientation.w = 1.0;
    place_pose.position.x = 0.0;
    place_pose.position.y = 0.4;
    place_pose.position.z = 0.5;
    move_group.setPoseTarget(place_pose);
    move_and_execute(move_group, "PLACE");

    rclcpp::sleep_for(std::chrono::seconds(1));

    // --- Step 6: Return HOME ---
    RCLCPP_INFO(this->get_logger(), "Step 6: Returning HOME...");
    move_group.setNamedTarget("ready");
    move_and_execute(move_group, "RETURN HOME");

    RCLCPP_INFO(this->get_logger(), "=== Pick and Place Complete! ===");
  }

private:
  void move_and_execute(
    moveit::planning_interface::MoveGroupInterface & move_group,
    const std::string & step_name)
  {
    moveit::planning_interface::MoveGroupInterface::Plan plan;
    bool success = (move_group.plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);

    if (success) {
      RCLCPP_INFO(this->get_logger(), "[%s] Plan successful! Executing...", step_name.c_str());
      move_group.execute(plan);
      RCLCPP_INFO(this->get_logger(), "[%s] Done!", step_name.c_str());
    } else {
      RCLCPP_ERROR(this->get_logger(), "[%s] Planning FAILED!", step_name.c_str());
    }
  }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<ArmController>();
  node->run_pick_and_place();
  rclcpp::shutdown();
  return 0;
}
