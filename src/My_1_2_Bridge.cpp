// Copyright 2015 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include <memory>
#include <utility>

// include ROS 1
#ifdef __clang__
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunused-parameter"
#endif
#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#ifdef __clang__
# pragma clang diagnostic pop
#endif

// include ROS 2
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"

float collected_data;
rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub;

void chatterCallback(const std_msgs::Float32MultiArray::ConstPtr & ros1_msg)
{
  // std::cout << "I heard: [" << ros1_msg->data << "]" << std::endl;

  auto ros2_msg = std::make_unique<std_msgs::msg::Float32MultiArray>();
  ros2_msg->data = ros1_msg->data;
  collected_data = ros1_msg->data[0];
  // std::cout << "Passing along: [" << ros2_msg->data << "]" << std::endl;
  RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "data : " << collected_data);
  pub->publish(std::move(ros2_msg));
}

int main(int argc, char * argv[])
{
  // ROS 2 node and publisher
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("talker");
  pub = node->create_publisher<std_msgs::msg::Float32MultiArray>("out", 10);

  // ROS 1 node and subscriber
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/robot1/pub_datas", 10, chatterCallback);

  ros::spin();

  return 0;
}
