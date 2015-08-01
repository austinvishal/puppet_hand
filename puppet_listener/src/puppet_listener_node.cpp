
#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/PoseStamped.h>
#include <sensor_msgs/JointState.h>
#include<baxter_core_msgs/JointCommand.h>
#include<baxter_core_msgs/SolvePositionIK.h>
#include<vector>
#include<iostream>
#include<sstream>

using namespace ros;
using namespace std;

Publisher joint_command ;
sensor_msgs::JointState desired_state;
baxter_core_msgs::JointCommand cmd;

	int main(int argc, char** argv)
	{
	  init(argc, argv, "transform_listener");
	  NodeHandle node;

	  ServiceClient client = node.serviceClient<baxter_core_msgs::SolvePositionIK>("/ExternalTools/left/PositionKinematicsNode/IKService");
	  joint_command = node.advertise<baxter_core_msgs::JointCommand>("/robot/limb/left/joint_command",1);

	  

	  tf::TransformListener listener;

	while( !client.exists() ) {
		ROS_INFO("Waiting for service to be up...");
		sleep(1.0) ;
	}
	  Rate rate(10);
	    while (ros::ok()){
	      tf::StampedTransform transform;
	     listener.waitForTransform("/base", "/vishprash",ros::Time(0),ros::Duration(2));
	     try{
	        listener.lookupTransform("/base", "/vishprash",ros::Time(0), transform);
	      }
	      catch (tf::TransformException ex){
	        ROS_ERROR("%s",ex.what());
	      }

	      geometry_msgs::PoseStamped pose;
	      pose.header.seq = 1234; 
	      pose.header.stamp = ros::Time::now();
	      pose.header.frame_id = "/base";
	      pose.pose.position.x=transform.getOrigin().x();
	      pose.pose.position.y=transform.getOrigin().y();
	      pose.pose.position.z=transform.getOrigin().z();
	      pose.pose.orientation.w=transform.getRotation().w();
	      pose.pose.orientation.x=transform.getRotation().x();
	      pose.pose.orientation.y=transform.getRotation().y();
	      pose.pose.orientation.z=transform.getRotation().z();

	      baxter_core_msgs::SolvePositionIK::Request srv_request;
	      baxter_core_msgs::SolvePositionIK::Response srv_response;

	      srv_request.pose_stamp.push_back(pose);
	      if (client.call(srv_request,srv_response))
	        {
			cout<<"Response :"<<srv_response;
	    	  if(srv_response.isValid[0])
	    	  {
			  ROS_INFO("Obtained a valid response");
	    		  desired_state = srv_response.joints[0];
				
			  cmd.mode=1;
			  for(int i = 0; i < desired_state.name.size();i++)
			  {	
	    		  	
	    		  	cmd.names.push_back(desired_state.name[i]);
	    		  	cmd.command.push_back(desired_state.position[i]);
	    		  }
				joint_command.publish(cmd);
	    	  }
	        }
	        else
	        {
	          ROS_ERROR("Failed to call service Inverse_Kinematic_chain");
	          return 1;
	        }


	      //publish

	      rate.sleep();
	    }
	    return 0;

}
