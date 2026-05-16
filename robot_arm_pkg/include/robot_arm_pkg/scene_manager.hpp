#ifndef ROBOT_ARM_PKG__SCENE_MANAGER_HPP_
#define ROBOT_ARM_PKG__SCENE_MANAGER_HPP_

#include <rclcpp/rclcpp.hpp>
#include <moveit/planning_scene_interface/planning_scene_interface.hpp>
#include <moveit_msgs/msg/collision_object.hpp>
#include <moveit_msgs/msg/object_color.hpp>
#include <shape_msgs/msg/solid_primitive.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <vector>
#include <string>

class SceneManager
{
public:
  explicit SceneManager(const rclcpp::Logger & logger);

  void setup_scene(moveit::planning_interface::PlanningSceneInterface & planning_scene);

private:
  rclcpp::Logger logger_;

  moveit_msgs::msg::CollisionObject make_box(
    const std::string & id,
    double x, double y, double z,
    double sx, double sy, double sz);

  moveit_msgs::msg::CollisionObject make_cylinder(
    const std::string & id,
    double x, double y, double z,
    double radius, double length);

  moveit_msgs::msg::ObjectColor make_color(
    const std::string & id,
    float r, float g, float b, float a = 1.0f);
};

#endif  // ROBOT_ARM_PKG__SCENE_MANAGER_HPP_
