#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <image_transport/image_transport.h>
//#include <move_base_msgs/MoveBaseAction.h>
//#include "simple_action_client.h"
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_listener.h>
 #include <tf/tf.h>
//#include "move_base_msgs/MoveBaseAction.h"
//This file *also* has been modified, but can serve as a starting point
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
int numRow;


void imageCallback(const sensor_msgs::ImageConstPtr& msg) {
    std::stringstream stream;

    cv_bridge::CvImagePtr cv_ptr;
        try
        {
          cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        }
        catch (cv_bridge::Exception& e)
        {
          ROS_ERROR("cv_bridge exception: %s", e.what());
          return;
        }
    stream << numRow;
    stream << ".jpeg";
    ROS_ASSERT( cv::imwrite( stream.str(),  cv_ptr->image ) );



}


int main(int argc, char **argv) {
  ros::init(argc, argv, "main");
  ros::NodeHandle n;

  //ros::Subscriber sub; //= n.subscribe("/kinect2/hd/image_color", 100, &ImageProc::imageCb);
  image_transport::ImageTransport it(n);
  image_transport::Subscriber sub;

  std::vector<geometry_msgs::PoseStamped> rowStops;
  std::stringstream ss;
   //std::string = ros::package::getPath("finalProject");

   ss << "input.txt";

  std::ifstream inFile("src/finalProject/src/input.txt", std::ios::in);

  //inFile.open("input.txt");
	std::cout << inFile.is_open();
    if (!inFile.is_open()) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
  numRow = 0;
  int currRow = 0;

  for(int row = 0; row < 10; row++) {
      geometry_msgs::PoseStamped pose;
      pose.header.frame_id = "/map";
      inFile >> pose.pose.position.x;
      inFile >> pose.pose.position.y;
      inFile >> pose.pose.position.z;
      inFile >> pose.pose.orientation.x;
      inFile >> pose.pose.orientation.y;
      inFile >> pose.pose.orientation.z;
      inFile >> pose.pose.orientation.w;
      rowStops.push_back(pose);
  }
  inFile.close();

  MoveBaseClient _ac("move_base", true);

  while(!_ac.waitForServer(ros::Duration(5.0))){
      ROS_INFO("Waiting for the move_base action server to come up");
   }

  // get robot to move to nav goals
 tf::TransformListener listener(ros::Duration(5));

  // get robot to move to nav goals
  while(currRow < (int)rowStops.size()) {
    //geometry_msgs::Pose p = rowStops.at(currRow);
    geometry_msgs::PoseStamped currPose = rowStops.at(currRow);
    ros::Time now = ros::Time::now();
    currPose.header.stamp = now;
    geometry_msgs::PoseStamped transformedPose;

    listener.waitForTransform("/base_link", currPose.header.frame_id, ros::Time(0), ros::Duration(5));

   ROS_INFO("\n%f\n%f\n%f\n", currPose.pose.position.x,currPose.pose.position.y, currPose.pose.position.z);
    listener.transformPose("/base_link", ros::Time(0), currPose,"/map", transformedPose);
   ROS_INFO("\n%f\n%f\n%f\n", transformedPose.pose.position.x,transformedPose.pose.position.y, transformedPose.pose.position.z);
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose = transformedPose;

    currRow++;

    //tell ros to wait
    ROS_INFO("Sending goal");
      	_ac.sendGoal(goal);
        numRow++;
     	_ac.waitForResult();
   	if(_ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
       	ROS_INFO("Hooray, the base moved");
    // we output the image
    sub = it.subscribe("/nav_kinect/rgb/image_color", 10, imageCallback);
    // wait 1 second
    ros::Duration(0.5).sleep();
    ros::spinOnce();
    sub.shutdown();
    }
    else {
       	ROS_INFO("The base failed to move forward");
    }



    }








  return 0;
}
