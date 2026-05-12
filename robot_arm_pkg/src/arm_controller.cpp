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
    RCLCPP_INFO(this->get_logger(), "=== Realistic Pick and Place Workspace ===");
  }

  void run()
  {
    auto arm = moveit::planning_interface::MoveGroupInterface(shared_from_this(), "panda_arm");
    auto gripper = moveit::planning_interface::MoveGroupInterface(shared_from_this(), "hand");
    moveit::planning_interface::PlanningSceneInterface planning_scene;

    arm.setMaxVelocityScalingFactor(0.15);
    arm.setMaxAccelerationScalingFactor(0.15);
    arm.setPlanningTime(15.0);
    arm.setNumPlanningAttempts(20);
    arm.setGoalPositionTolerance(0.005);
    arm.setGoalOrientationTolerance(0.02);

    RCLCPP_INFO(this->get_logger(), "Building workspace...");
    add_scene_objects(planning_scene);
    rclcpp::sleep_for(std::chrono::seconds(2));

    RCLCPP_INFO(this->get_logger(), "Opening gripper...");
    gripper.setNamedTarget("open");
    gripper.move();
    rclcpp::sleep_for(std::chrono::milliseconds(800));

    RCLCPP_INFO(this->get_logger(), "Moving to home...");
    arm.setNamedTarget("ready");
    move_and_execute(arm, "HOME");
    rclcpp::sleep_for(std::chrono::milliseconds(800));

    RCLCPP_INFO(this->get_logger(), "Approaching package...");
    geometry_msgs::msg::Pose pre_grasp;
    pre_grasp.orientation.w = 0.0;
    pre_grasp.orientation.x = 1.0;
    pre_grasp.orientation.y = 0.0;
    pre_grasp.orientation.z = 0.0;
    pre_grasp.position.x = 0.45;
    pre_grasp.position.y = 0.0;
    pre_grasp.position.z = 0.55;
    arm.setPoseTarget(pre_grasp);
    move_and_execute(arm, "APPROACH");
    rclcpp::sleep_for(std::chrono::milliseconds(800));

    RCLCPP_INFO(this->get_logger(), "Lowering...");
    geometry_msgs::msg::Pose grasp_pose = pre_grasp;
    grasp_pose.position.z = 0.39;
    cartesian_move(arm, grasp_pose, "LOWER", 0.003);
    rclcpp::sleep_for(std::chrono::milliseconds(600));

    RCLCPP_INFO(this->get_logger(), "Gripping...");
    gripper.setNamedTarget("close");
    gripper.move();
    rclcpp::sleep_for(std::chrono::milliseconds(1000));

    arm.attachObject("box", "panda_link8", {"panda_leftfinger", "panda_rightfinger"});
    rclcpp::sleep_for(std::chrono::milliseconds(400));

    RCLCPP_INFO(this->get_logger(), "Lifting...");
    geometry_msgs::msg::Pose lift_pose = grasp_pose;
    lift_pose.position.z = 0.65;
    cartesian_move(arm, lift_pose, "LIFT", 0.003);
    rclcpp::sleep_for(std::chrono::milliseconds(800));

    RCLCPP_INFO(this->get_logger(), "Transporting...");
    geometry_msgs::msg::Pose above_t2;
    above_t2.orientation.w = 0.0;
    above_t2.orientation.x = 1.0;
    above_t2.orientation.y = 0.0;
    above_t2.orientation.z = 0.0;
    above_t2.position.x = 0.0;
    above_t2.position.y = 0.50;
    above_t2.position.z = 0.65;
    arm.setPoseTarget(above_t2);
    move_and_execute(arm, "TRANSPORT");
    rclcpp::sleep_for(std::chrono::milliseconds(800));

    RCLCPP_INFO(this->get_logger(), "Placing...");
    geometry_msgs::msg::Pose place_pose = above_t2;
    place_pose.position.z = 0.45;
    cartesian_move(arm, place_pose, "PLACE", 0.003);
    rclcpp::sleep_for(std::chrono::milliseconds(600));

    RCLCPP_INFO(this->get_logger(), "Releasing...");
    gripper.setNamedTarget("open");
    gripper.move();
    rclcpp::sleep_for(std::chrono::milliseconds(800));

    arm.detachObject("box");
    rclcpp::sleep_for(std::chrono::milliseconds(400));

    RCLCPP_INFO(this->get_logger(), "Retreating...");
    geometry_msgs::msg::Pose retreat = place_pose;
    retreat.position.z = 0.65;
    cartesian_move(arm, retreat, "RETREAT", 0.003);
    rclcpp::sleep_for(std::chrono::milliseconds(800));

    RCLCPP_INFO(this->get_logger(), "Home...");
    arm.setNamedTarget("ready");
    move_and_execute(arm, "HOME");
    gripper.setNamedTarget("close");
    gripper.move();

    RCLCPP_INFO(this->get_logger(), "=== Mission Complete! ===");
  }

private:
  void add_scene_objects(moveit::planning_interface::PlanningSceneInterface & planning_scene)
  {
    std::vector<moveit_msgs::msg::CollisionObject> objects;
    std::vector<moveit_msgs::msg::ObjectColor> colors;

    // Floor (dark wood)
    moveit_msgs::msg::CollisionObject floor;
    floor.header.frame_id = "panda_link0";
    floor.id = "floor";
    shape_msgs::msg::SolidPrimitive fl;
    fl.type = fl.BOX;
    fl.dimensions = {3.0, 3.0, 0.02};
    geometry_msgs::msg::Pose fl_pose;
    fl_pose.orientation.w = 1.0;
    fl_pose.position.x = 0.0;
    fl_pose.position.y = 0.0;
    fl_pose.position.z = -0.02;
    floor.primitives.push_back(fl);
    floor.primitive_poses.push_back(fl_pose);
    floor.operation = floor.ADD;
    objects.push_back(floor);
    moveit_msgs::msg::ObjectColor cf;
    cf.id = "floor";
    cf.color.r = 0.35; cf.color.g = 0.25; cf.color.b = 0.18; cf.color.a = 1.0;
    colors.push_back(cf);

    // Back wall
    moveit_msgs::msg::CollisionObject wall;
    wall.header.frame_id = "panda_link0";
    wall.id = "wall";
    shape_msgs::msg::SolidPrimitive w;
    w.type = w.BOX;
    w.dimensions = {3.0, 0.05, 1.5};
    geometry_msgs::msg::Pose w_pose;
    w_pose.orientation.w = 1.0;
    w_pose.position.x = 0.0;
    w_pose.position.y = -1.0;
    w_pose.position.z = 0.75;
    wall.primitives.push_back(w);
    wall.primitive_poses.push_back(w_pose);
    wall.operation = wall.ADD;
    objects.push_back(wall);
    moveit_msgs::msg::ObjectColor cw;
    cw.id = "wall";
    cw.color.r = 0.85; cw.color.g = 0.82; cw.color.b = 0.75; cw.color.a = 1.0;
    colors.push_back(cw);

    // Robot base platform
    moveit_msgs::msg::CollisionObject base;
    base.header.frame_id = "panda_link0";
    base.id = "robot_base";
    shape_msgs::msg::SolidPrimitive bs;
    bs.type = bs.BOX;
    bs.dimensions = {0.3, 0.3, 0.05};
    geometry_msgs::msg::Pose bs_pose;
    bs_pose.orientation.w = 1.0;
    bs_pose.position.x = 0.0;
    bs_pose.position.y = 0.0;
    bs_pose.position.z = -0.025;
    base.primitives.push_back(bs);
    base.primitive_poses.push_back(bs_pose);
    base.operation = base.ADD;
    objects.push_back(base);
    moveit_msgs::msg::ObjectColor cbs;
    cbs.id = "robot_base";
    cbs.color.r = 0.3; cbs.color.g = 0.3; cbs.color.b = 0.35; cbs.color.a = 1.0;
    colors.push_back(cbs);

    // Table 1 - BLACK
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

    // Table 2 - BLACK (taller)
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
    c2.color.r = 0.05; c2.color.g = 0.05; c2.color.b = 0.05; c2.color.a = 1.0;
    colors.push_back(c2);

    // BLUE Box
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
    cb.color.r = 0.0; cb.color.g = 0.4; cb.color.b = 1.0; cb.color.a = 1.0;
    colors.push_back(cb);

    // Decorative vase
    moveit_msgs::msg::CollisionObject vase;
    vase.header.frame_id = "panda_link0";
    vase.id = "vase";
    shape_msgs::msg::SolidPrimitive v;
    v.type = v.CYLINDER;
    v.dimensions = {0.25, 0.06};
    geometry_msgs::msg::Pose v_pose;
    v_pose.orientation.w = 1.0;
    v_pose.position.x = -0.5;
    v_pose.position.y = -0.7;
    v_pose.position.z = 0.125;
    vase.primitives.push_back(v);
    vase.primitive_poses.push_back(v_pose);
    vase.operation = vase.ADD;
    objects.push_back(vase);
    moveit_msgs::msg::ObjectColor cv;
    cv.id = "vase";
    cv.color.r = 0.5; cv.color.g = 0.15; cv.color.b = 0.15; cv.color.a = 1.0;
    colors.push_back(cv);

    planning_scene.applyCollisionObjects(objects, colors);
    RCLCPP_INFO(this->get_logger(), "Workspace ready: floor, wall, 2 black tables, blue box, vase!");
  }

  void move_and_execute(
    moveit::planning_interface::MoveGroupInterface & move_group,
    const std::string & step_name)
  {
    moveit::planning_interface::MoveGroupInterface::Plan plan;
    bool success = (move_group.plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);
    if (success) {
      RCLCPP_INFO(this->get_logger(), "[%s] Executing...", step_name.c_str());
      move_group.execute(plan);
      RCLCPP_INFO(this->get_logger(), "[%s] Done.", step_name.c_str());
    } else {
      RCLCPP_ERROR(this->get_logger(), "[%s] Planning failed!", step_name.c_str());
    }
  }

  void cartesian_move(
    moveit::planning_interface::MoveGroupInterface & move_group,
    const geometry_msgs::msg::Pose & target,
    const std::string & step_name,
    double step_size = 0.005)
  {
    std::vector<geometry_msgs::msg::Pose> waypoints;
    waypoints.push_back(target);
    moveit_msgs::msg::RobotTrajectory traj;
    double fraction = move_group.computeCartesianPath(waypoints, step_size, traj);
    if (fraction > 0.7) {
      RCLCPP_INFO(this->get_logger(), "[%s] Smooth %.0f%%", step_name.c_str(), fraction * 100);
      move_group.execute(traj);
      RCLCPP_INFO(this->get_logger(), "[%s] Done.", step_name.c_str());
    } else {
      RCLCPP_WARN(this->get_logger(), "[%s] Fallback planning...", step_name.c_str());
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
