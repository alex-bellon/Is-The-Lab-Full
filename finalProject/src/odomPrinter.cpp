#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include <iostream>

using namespace std;
void chatterCallback(const nav_msgs::Odometry::ConstPtr& msg)
 {
    char key = getchar();
    int asciiValue = key;
    if(key = 97) {
    ROS_INFO("Printing data:");
   ROS_INFO("\n%f\n%f\n%f\n", msg->pose.pose.position.x,msg->pose.pose.position.y, msg->pose.pose.position.z);
   ROS_INFO("\n%f\n%f\n%f\n%f\n", msg->pose.pose.orientation.x, msg->pose.pose.orientation.y, msg->pose.pose.orientation.z, msg->pose.pose.orientation.w);
   }
 }

//This file *also* has been modified, but can serve as a starting point
int main(int argc, char **argv) {
  ros::init(argc, argv, "odomPrinter");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("odom", 1000, chatterCallback);
  ros::spin();
  return 0;
}
