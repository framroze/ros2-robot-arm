#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

class ArmController : public rclcpp::Node
{
public:
  ArmController() : Node("arm_controller")
  {
    RCLCPP_INFO(this->get_logger(), "Robot Arm Controller Node Started!");
    
    publisher_ = this->create_publisher<std_msgs::msg::String>("arm_status", 10);
    
    timer_ = this->create_wall_timer(
      std::chrono::milliseconds(1000),
      std::bind(&ArmController::publish_status, this));
  }

private:
  void publish_status()
  {
    auto message = std_msgs::msg::String();
    message.data = "Robot arm is ready!";
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    publisher_->publish(message);
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ArmController>());
  rclcpp::shutdown();
  return 0;
}
