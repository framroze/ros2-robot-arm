#include "robot_arm_pkg/scene_manager.hpp"

SceneManager::SceneManager(const rclcpp::Logger & logger)
: logger_(logger)
{
}

void SceneManager::setup_scene(
  moveit::planning_interface::PlanningSceneInterface & planning_scene)
{
  std::vector<moveit_msgs::msg::CollisionObject> objects;
  std::vector<moveit_msgs::msg::ObjectColor> colors;

  // --- Floor (dark wood) ---
  auto floor = make_box("floor", 0.0, 0.0, -0.02, 3.0, 3.0, 0.02);
  objects.push_back(floor);
  colors.push_back(make_color("floor", 0.35f, 0.25f, 0.18f));

  // --- Back wall ---
  auto wall = make_box("wall", 0.0, -1.0, 0.75, 3.0, 0.05, 1.5);
  objects.push_back(wall);
  colors.push_back(make_color("wall", 0.85f, 0.82f, 0.75f));

  // --- Robot base platform ---
  auto base = make_box("robot_base", 0.0, 0.0, -0.025, 0.3, 0.3, 0.05);
  objects.push_back(base);
  colors.push_back(make_color("robot_base", 0.3f, 0.3f, 0.35f));

  // --- Table 1 (black, shorter) - pick location ---
  auto table1 = make_box("table1", 0.50, 0.0, 0.10, 0.4, 0.4, 0.20);
  objects.push_back(table1);
  colors.push_back(make_color("table1", 0.08f, 0.08f, 0.08f));

  // --- Table 2 (black, taller) - place location ---
  auto table2 = make_box("table2", 0.0, 0.50, 0.135, 0.35, 0.35, 0.27);
  objects.push_back(table2);
  colors.push_back(make_color("table2", 0.05f, 0.05f, 0.05f));

  // --- Blue box (package to pick) ---
  auto box = make_box("box", 0.45, 0.0, 0.23, 0.06, 0.06, 0.06);
  objects.push_back(box);
  colors.push_back(make_color("box", 0.0f, 0.4f, 1.0f));

  // --- Decorative vase ---
  auto vase = make_cylinder("vase", -0.5, -0.7, 0.125, 0.06, 0.25);
  objects.push_back(vase);
  colors.push_back(make_color("vase", 0.5f, 0.15f, 0.15f));

  planning_scene.applyCollisionObjects(objects, colors);

  RCLCPP_INFO(logger_, "Scene ready: floor, wall, 2 black tables, blue box, vase!");
}

moveit_msgs::msg::CollisionObject SceneManager::make_box(
  const std::string & id,
  double x, double y, double z,
  double sx, double sy, double sz)
{
  moveit_msgs::msg::CollisionObject obj;
  obj.header.frame_id = "panda_link0";
  obj.id = id;

  shape_msgs::msg::SolidPrimitive shape;
  shape.type = shape.BOX;
  shape.dimensions = {sx, sy, sz};

  geometry_msgs::msg::Pose pose;
  pose.orientation.w = 1.0;
  pose.position.x = x;
  pose.position.y = y;
  pose.position.z = z;

  obj.primitives.push_back(shape);
  obj.primitive_poses.push_back(pose);
  obj.operation = obj.ADD;

  return obj;
}

moveit_msgs::msg::CollisionObject SceneManager::make_cylinder(
  const std::string & id,
  double x, double y, double z,
  double radius, double length)
{
  moveit_msgs::msg::CollisionObject obj;
  obj.header.frame_id = "panda_link0";
  obj.id = id;

  shape_msgs::msg::SolidPrimitive shape;
  shape.type = shape.CYLINDER;
  shape.dimensions = {length, radius};

  geometry_msgs::msg::Pose pose;
  pose.orientation.w = 1.0;
  pose.position.x = x;
  pose.position.y = y;
  pose.position.z = z;

  obj.primitives.push_back(shape);
  obj.primitive_poses.push_back(pose);
  obj.operation = obj.ADD;

  return obj;
}

moveit_msgs::msg::ObjectColor SceneManager::make_color(
  const std::string & id,
  float r, float g, float b, float a)
{
  moveit_msgs::msg::ObjectColor color;
  color.id = id;
  color.color.r = r;
  color.color.g = g;
  color.color.b = b;
  color.color.a = a;
  return color;
}
