#include "ros/ros.h"
#include <sensor_msgs/Joy.h>
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"
#include <string>

const double Rw=1.0;    //タイヤ半径
const double T=1.0;     //トレッド

double vel;
double avel;
double wr_tmp,wl_tmp;
int wr, wl;
int r_flg,l_flg;
geometry_msgs::Twist cmd;

void joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
    //ros::NodeHandle n;
    //ROS_INFO("speed set\n");
    vel =   joy->axes[1];
    avel =  joy->axes[2];
    wr_tmp = ((1/Rw)*vel + (T/(2*Rw))*avel)*255/1.5;
    wl_tmp = ((1/Rw)*vel - (T/(2*Rw))*avel)*255/1.5;
    if(wr_tmp>0)
    {
        r_flg=1;
        wr = wr_tmp;
    }
    else
    {
        r_flg=0;
        wr = -wr_tmp;
    }
    if(wl_tmp>0)
    {
        l_flg=1;
        wl = wl_tmp;
    }
    else
    {
        l_flg=0;
        wl = -wl_tmp;
    }
}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "set_velocity_node");
    ros::NodeHandle n;

	ros::Subscriber sub = n.subscribe("joy", 1000, joyCallback);
    ros::Publisher pub_twist = n.advertise<geometry_msgs::Twist>("cmd_vel", 50);

    ros::Rate loop_rate(20);

	vel = 0;
    avel = 0;
	while(ros::ok())
	{
        //ROS_INFO("linear: %f,  angular: %f", vel, avel);//debug
        cmd.linear.x =  vel;
        cmd.linear.y = r_flg;
        cmd.linear.z = l_flg;
        cmd.angular.z = avel;
        cmd.angular.x = wr;     //右モータ
        cmd.angular.y = wl;     //左モータ
        pub_twist.publish(cmd);


		ros::spinOnce();

		loop_rate.sleep();
	}
	return 0;
}
