/*
 * Copyright (c) 2010, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

// %Tag(FULLTEXT)%
// %Tag(INCLUDES)%
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include "tf/tf.h"
#include "nav_msgs/Odometry.h"
#include <math.h>
// %EndTag(INCLUDES)%

float pose_x = 0.0, pose_y = 0.0;
void get_pose(const nav_msgs::Odometry::ConstPtr& msg);

// %Tag(INIT)%
int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  ros::Subscriber obom_sub = n.subscribe("/odom", 1, get_pose);
// %EndTag(INIT)%

// Set our initial shape type to be a cube
// %Tag(SHAPE_INIT)%
uint32_t shape = visualization_msgs::Marker::CUBE;
// %EndTag(SHAPE_INIT)%
  float target[2][3] = {{4,4,0.70707},
                        {-4,-4,-1.57079}
                        }; 
// %Tag(MARKER_INIT)%

  visualization_msgs::Marker marker;
  // Set the frame ID and timestamp.  See the TF tutorials for information on these.
  marker.header.frame_id = "/map";
  marker.header.stamp = ros::Time::now();
// %EndTag(MARKER_INIT)%

  // Set the namespace and id for this marker.  This serves to create a unique ID
  // Any marker sent with the same namespace and id will overwrite the old one
// %Tag(NS_ID)%
  marker.ns = "add_markers";
  marker.id = 0;
// %EndTag(NS_ID)%

  // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
// %Tag(TYPE)%
  marker.type = shape;
// %EndTag(TYPE)%

  // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
// %Tag(ACTION)%
  marker.action = visualization_msgs::Marker::ADD;
// %EndTag(ACTION)%

  // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
// %Tag(POSE)%
  int idx = 0;
  marker.pose.position.x = target[idx][0];
  marker.pose.position.y = target[idx][1];
  marker.pose.position.z = 0.2;
  marker.pose.orientation= tf::createQuaternionMsgFromYaw(target[idx][2]);
// %EndTag(POSE)%

  // Set the scale of the marker -- 1x1x1 here means 1m on a side
// %Tag(SCALE)%
  marker.scale.x = 0.2;
  marker.scale.y = 0.2;
  marker.scale.z = 0.2;
// %EndTag(SCALE)%

  // Set the color -- be sure to set alpha to something non-zero!
// %Tag(COLOR)%
  marker.color.r = 0.0f;
  marker.color.g = 1.0f;
  marker.color.b = 0.0f;
  marker.color.a = 1.0;
// %EndTag(COLOR)%

// %Tag(LIFETIME)%
  marker.lifetime = ros::Duration();
// %EndTag(LIFETIME)%

float error = fabs(target[idx][0] - pose_x) + fabs(target[idx][1] - pose_y);
float threshold = 0.3;
while (ros::ok() && idx < 2)
{
    // Publish the marker
// %Tag(PUBLISH)%
    marker_pub.publish(marker);
// %EndTag(PUBLISH)%
  error = fabs(target[idx][10] - pose_x) + fabs(target[idx][1] - pose_y);
  if (error < threshold)
  {
    marker.action = visualization_msgs::Marker::DELETE;
    marker_pub.publish(marker);
    ROS_INFO("Object trtrieved succesfully!");
    idx++;
    marker.action = visualization_msgs::Marker::ADD;
    marker.pose.position.x = target[idx][0];
    marker.pose.position.y = target[idx][1];
    marker.pose.position.z = 0.2;
    marker.pose.orientation= tf::createQuaternionMsgFromYaw(target[idx][2]);
  }
// %Tag(SLEEP_END)%
    r.sleep();
  }
// %EndTag(SLEEP_END)%
}
// %EndTag(FULLTEXT)%

void get_pose(const nav_msgs::Odometry::ConstPtr& msg)
{
  ::pose_x = msg->pose.pose.position.x;
  ::pose_y = msg->pose.pose.position.y;
}