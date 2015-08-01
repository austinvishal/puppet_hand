#include <ros/ros.h>

#include <tf/transform_broadcaster.h>


using namespace ros;
using namespace std;

void addtf()
{
	static tf::TransformBroadcaster br;
	tf::Transform transform;
	
	transform.setOrigin(tf::Vector3(0.0, 0.0, 0.2));
    	transform.setRotation(tf::Quaternion(M_PI, 0,0 ));

    	br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "/right_gripper", "/vishprash"));
}


int main(int argc, char** argv)
{
  init(argc, argv, "puppet_hand_broadcaster");
  NodeHandle node("~");
  Rate loop_rate(10);
  
  while (ros::ok()){
	ros::spinOnce();
	addtf();
	loop_rate.sleep();
	
	}
    
  return 0;
}
