#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.hpp>
#include <moveit/planning_scene_interface/planning_scene_interface.hpp>
#include <moveit_msgs/msg/collision_object.hpp>
#include <moveit_msgs/msg/object_color.hpp>
#include <shape_msgs/msg/solid_primitive.hpp>
#include <geometry_msgs/msg/pose.hpp>

class PickAndPlace : public rclcpp::Node
{
public:
  PickAndPlace() : Node("pick_and_place", rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true))
  {
    RCLCPP_INFO(this->get_logger(), "=== Pick and Place ===");
  }

  void run()
  {
    auto arm = moveit::planning_interface::MoveGroupInterface(shared_from_this(), "panda_arm");
    auto gripper = moveit::planning_interface::MoveGroupInterface(shared_from_this(), "hand");
    moveit::planning_interface::PlanningSceneInterface planning_scene;

    arm.setMaxVelocityScalingFactor(0.3);
    arm.setMaxAccelerationScalingFactor(0.3);
    arm.setPlanningTime(15.0);
    arm.setNumPlanningAttempts(10);

    // Step 1: Scene
    RCLCPP_INFO(this->get_logger(), "Step 1: Setting up scene...");
    add_scene_objects(planning_scene);
    rclcpp::sleep_for(std::chrono::seconds(2));

    // Step 2: Open gripper
    RCLCPP_INFO(this->get_logger(), "Step 2: Opening gripper...");
    gripper.setNamedTarget("open");
    gripper.move();
    rclcpp::sleep_for(std::chrono::milliseconds(500));

    // Step 3: HOME
    RCLCPP_INFO(this->get_logger(), "Step 3: Moving HOME...");
    arm.setNamedTarget("ready");
    move_and_execute(arm, "HOME");
    rclcpp::sleep_for(std::chrono::milliseconds(500));

    // Step 4: Above box (pre-grasp)
    RCLCPP_INFO(this->get_logger(), "Step 4: Moving ABOVE box (pre-grasp)...");
    geometry_msgs::msg::Pose pre_grasp;
    pre_grasp.orientation.w = 0.0;
    pre_grasp.orientation.x = 1.0;
    pre_grasp.orientation.y = 0.0;
    pre_grasp.orientation.z = 0.0;
    pre_grasp.position.x = 0.45;
    pre_grasp.position.y = 0.0;
    pre_grasp.position.z = 0.55;
    arm.setPoseTarget(pre_grasp);
    move_and_execute(arm, "PRE-GRASP");
    rclcpp::sleep_for(std::chrono::milliseconds(500));

    // Step 5: Allow touch between gripper and box, then move down with Cartesian path
    RCLCPP_INFO(this->get_logger(), "Step 5: Lowering to grasp (Cartesian)...");
    geometry_msgs::msg::Pose grasp_pose = pre_grasp;
    grasp_pose.position.z = 0.39;  // Box top at z=0.27, gripper tip just above
    cartesian_move(arm, grasp_pose, "LOWER TO GRASP");
    rclcpp::sleep_for(std::chrono::milliseconds(500));

    // Step 6: Close gripper
    RCLCPP_INFO(this->get_logger(), "Step 6: Closing gripper...");
    gripper.setNamedTarget("close");
    gripper.move();
    RCLCPP_INFO(this->get_logger(), "Box GRASPED!");
    rclcpp::sleep_for(std::chrono::milliseconds(800));

    // Step 7: Attach box
    arm.attachObject("box", "panda_link8", {"panda_leftfinger", "panda_rightfinger"});
    rclcpp::sleep_for(std::chrono::milliseconds(300));

    // Step 8: Lift up Cartesian
    RCLCPP_INFO(this->get_logger(), "Step 8: Lifting box up (Cartesian)...");
    geometry_msgs::msg::Pose lift_pose = grasp_pose;
    lift_pose.position.z = 0.65;
    cartesian_move(arm, lift_pose, "LIFT UP");
    rclcpp::sleep_for(std::chrono::milliseconds(500));

    // Step 9: Move above Table 2
    RCLCPP_INFO(this->get_logger(), "Step 9: Moving above Table 2...");
    geometry_msgs::msg::Pose above_t2;
    above_t2.orientation.w = 0.0;
    above_t2.orientation.x = 1.0;
    above_t2.orientation.y = 0.0;
    above_t2.orientation.z = 0.0;
    above_t2.position.x = 0.0;
    above_t2.position.y = 0.50;
    above_t2.position.z = 0.65;
    arm.setPoseTarget(above_t2);
    move_and_execute(arm, "ABOVE TABLE 2");
    rclcpp::sleep_for(std::chrono::milliseconds(500));

    // Step 10: Lower to place Cartesian
    RCLCPP_INFO(this->get_logger(), "Step 10: Lowering to place (Cartesian)...");
    geometry_msgs::msg::Pose place_pose = above_t2;
    place_pose.position.z = 0.45;  // Table 2 top at z=0.27, place at top
    cartesian_move(arm, place_pose, "PLACE DOWN");
    rclcpp::sleep_for(std::chrono::milliseconds(500));

    // Step 11: Open gripper
    RCLCPP_INFO(this->get_logger(), "Step 11: Releasing box...");
    gripper.setNamedTarget("open");
    gripper.move();
    rclcpp::sleep_for(std::chrono::milliseconds(500));

    // Step 12: Detach
    arm.detachObject("box");
    rclcpp::sleep_for(std::chrono::milliseconds(300));

    // Step 13: Lift up Cartesian
    RCLCPP_INFO(this->get_logger(), "Step 13: Lifting away...");
    geometry_msgs::msg::Pose retreat = place_pose;
    retreat.position.z = 0.65;
    cartesian_move(arm, retreat, "LIFT AWAY");
    rclcpp::sleep_for(std::chrono::milliseconds(500));

    // Step 14: Home
    RCLCPP_INFO(this->get_logger(), "Step 14: Returning HOME...");
    arm.setNamedTarget("ready");
    move_and_execute(arm, "RETURN HOME");
    gripper.setNamedTarget("close");
    gripper.move();

    RCLCPP_INFO(this->get_logger(), "=== Pick and Place COMPLETE! ===");
  }

private:
  void add_scene_objects(moveit::planning_interface::PlanningSceneInterface & planning_scene)
  {
    std::vector<moveit_msgs::msg::CollisionObject> objects;
    std::vector<moveit_msgs::msg::ObjectColor> colors;

    // Table 1 (shorter): height=0.20, top at z=0.20
    moveit_msgs::msg::CollisionObject table1;
    table1.header.frame_id = "panda_link0";
    table1.id = "table1";
    shape_msgs::msg::SolidPrimitive t1;
    t1.type = t1.BOX;
    t1.dimensions = {0.4, 0.4, 0.20};
    geometry_msgs::msg::Pose t1_pose;
    t1_pose.orientation.w = 1.0;
    t1_pose.position.x = 0.50;
    t1_pose.position.y = 0.0;
    t1_pose.position.z = 0.10;
    table1.primitives.push_back(t1);
    table1.primitive_poses.push_back(t1_pose);
    table1.operation = table1.ADD;
    objects.push_back(table1);
    moveit_msgs::msg::ObjectColor c1;
    c1.id = "table1";
    c1.color.r = 0.08; c1.color.g = 0.08; c1.color.b = 0.08; c1.color.a = 1.0;
    colors.push_back(c1);

    // Table 2 (taller): height=0.27, top at z=0.27
    moveit_msgs::msg::CollisionObject table2;
    table2.header.frame_id = "panda_link0";
    table2.id = "table2";
    shape_msgs::msg::SolidPrimitive t2;
    t2.type = t2.BOX;
    t2.dimensions = {0.35, 0.35, 0.27};
    geometry_msgs::msg::Pose t2_pose;
    t2_pose.orientation.w = 1.0;
    t2_pose.position.x = 0.0;
    t2_pose.position.y = 0.50;
    t2_pose.position.z = 0.135;
    table2.primitives.push_back(t2);
    table2.primitive_poses.push_back(t2_pose);
    table2.operation = table2.ADD;
    objects.push_back(table2);
    moveit_msgs::msg::ObjectColor c2;
    c2.id = "table2";
    c2.color.r = 0.08; c2.color.g = 0.08; c2.color.b = 0.08; c2.color.a = 1.0;
    colors.push_back(c2);

    // Blue box on table 1: box height=0.06, sits on table1 top (z=0.20), center=0.23
    moveit_msgs::msg::CollisionObject box;
    box.header.frame_id = "panda_link0";
    box.id = "box";
    shape_msgs::msg::SolidPrimitive b;
    b.type = b.BOX;
    b.dimensions = {0.06, 0.06, 0.06};
    geometry_msgs::msg::Pose b_pose;
    b_pose.orientation.w = 1.0;
    b_pose.position.x = 0.45;
    b_pose.position.y = 0.0;
    b_pose.position.z = 0.23;
    box.primitives.push_back(b);
    box.primitive_poses.push_back(b_pose);
    box.operation = box.ADD;
    objects.push_back(box);
    moveit_msgs::msg::ObjectColor cb;
    cb.id = "box";
    cb.color.r = 0.0; cb.color.g = 0.3; cb.color.b = 1.0; cb.color.a = 1.0;
    colors.push_back(cb);

    planning_scene.applyCollisionObjects(objects, colors);
    RCLCPP_INFO(this->get_logger(), "Scene: 2 black tables (heights 0.20 + 0.27) + 1 blue box on Table 1");
  }

  void move_and_execute(
    moveit::planning_interface::MoveGroupInterface & move_group,
    const std::string & step_name)
  {
    moveit::planning_interface::MoveGroupInterface::Plan plan;
    bool success = (move_group.plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);
    if (success) {
      RCLCPP_INFO(this->get_logger(), "[%s] Plan OK!", step_name.c_str());
      move_group.execute(plan);
      RCLCPP_INFO(this->get_logger(), "[%s] Done!", step_name.c_str());
    } else {
      RCLCPP_ERROR(this->get_logger(), "[%s] Planning FAILED!", step_name.c_str());
    }
  }

  void cartesian_move(
    moveit::planning_interface::MoveGroupInterface & move_group,
    const geometry_msgs::msg::Pose & target,
    const std::string & step_name)
  {
    std::vector<geometry_msgs::msg::Pose> waypoints;
    waypoints.push_back(target);

    moveit_msgs::msg::RobotTrajectory traj;
    double fraction = move_group.computeCartesianPath(waypoints, 0.005, traj);

    if (fraction > 0.7) {
      RCLCPP_INFO(this->get_logger(), "[%s] Cartesian path %.0f%% OK!", step_name.c_str(), fraction * 100);
      move_group.execute(traj);
      RCLCPP_INFO(this->get_logger(), "[%s] Done!", step_name.c_str());
    } else {
      RCLCPP_WARN(this->get_logger(), "[%s] Cartesian only %.0f%%, trying regular plan...", step_name.c_str(), fraction * 100);
      move_group.setPoseTarget(target);
      move_and_execute(move_group, step_name);
    }
  }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<PickAndPlace>();
  node->run();
  rclcpp::shutdown();
  return 0;
}
