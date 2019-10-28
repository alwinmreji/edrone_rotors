/*
 * Copyright 2015 Fadri Furrer, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Michael Burri, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Mina Kamel, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Janosch Nikolic, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Markus Achtelik, ASL, ETH Zurich, Switzerland
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


// #include "rotors_edrone_interface/edrone_trans.h"

// #include <mav_msgs/default_topics.h>

// Edrone_Trans::Edrone_Trans() {
//   ros::NodeHandle nh;
//   ros::NodeHandle pnh("~");
//   ctrl_pub_ = nh_.advertise<mav_msgs::RollPitchYawrateThrust> (
//     mav_msgs::default_topics::COMMAND_ROLL_PITCH_YAWRATE_THRUST, 50);

//   control_msg_.roll = 0;
//   control_msg_.pitch = 0;
//   control_msg_.yaw_rate = 0;
//   control_msg_.thrust.x = 0;
//   control_msg_.thrust.y = 0;
//   control_msg_.thrust.z = 0;
//   current_yaw_vel_ = 0;

//   pnh.param("axis_direction_roll", axes_.roll_direction, 1);
//   pnh.param("axis_direction_pitch", axes_.pitch_direction, 1);
//   pnh.param("axis_direction_thrust", axes_.thrust_direction, 1);

//   pnh.param("max_v_xy", max_.v_xy, 0.8);  // [m/s]
//   pnh.param("max_roll", max_.roll, 8.5 * M_PI / 180.0);  // [rad]
//   pnh.param("max_pitch", max_.pitch, 8.5 * M_PI / 180.0);  // [rad]
//   pnh.param("max_yaw_rate", max_.rate_yaw, 45.0 * M_PI / 180.0);  // [rad/s]
//   pnh.param("max_thrust", max_.thrust, 30.0);  // [N]
//   pnh.param("division_factor", div_factor, 85.0);  
//   pnh.param("v_yaw_step", v_yaw_step_, 0.05);  // [rad/s]
//   pnh.param("v_yaw_step", v_yaw_step_, 0.05);
//   pnh.param("thrust_weight_offset_newtons",thrust_weight_offset_newtons_,14.896);//14.896);
//   edrone_sub_ = nh_.subscribe("/drone_command", 50, &Edrone_Trans::TransCallback, this);
//   namespace_ = nh_.getNamespace();
// }

// void Edrone_Trans::StopMav() {
//   control_msg_.roll = 0;
//   control_msg_.pitch = 0;
//   control_msg_.yaw_rate = 0;
//   control_msg_.thrust.x = 0;
//   control_msg_.thrust.y = 0;
//   control_msg_.thrust.z = 0;
// }

// void Edrone_Trans::TransCallback(const edrone_client::edrone_msgs::ConstPtr& msg) {
//   control_msg_.roll = ((msg->rcRoll-1500) * max_.roll/div_factor)*axes_.roll_direction;
//   control_msg_.pitch = ((msg->rcPitch-1500) * max_.pitch/div_factor)* axes_.pitch_direction;
//   control_msg_.yaw_rate = current_yaw_vel_;
//   control_msg_.thrust.z = ((msg->rcThrottle-1500) * max_.thrust / 50.0 * axes_.thrust_direction)+thrust_weight_offset_newtons_;
//   ros::Time update_time = ros::Time::now();
//   control_msg_.header.stamp = update_time;
//   control_msg_.header.frame_id = "rotors_edrone_frame";
//   Publish();
// }

// void Edrone_Trans::Publish() {
//   ctrl_pub_.publish(control_msg_);
// }
// int main(int argc, char** argv) {
//   ros::init(argc, argv, "rotors_edrone_interface");
//   Edrone_Trans edrone_trans;
//   ros::spin();
//   return 0;
// }



#include "rotors_edrone_interface/edrone_trans.h"

#include <mav_msgs/default_topics.h>

Edrone_Trans::Edrone_Trans() {
  ros::NodeHandle nh;
  ros::NodeHandle pnh("~");
  ctrl_pub_ = nh_.advertise<mav_msgs::RollPitchYawrateThrust> (
    mav_msgs::default_topics::COMMAND_ROLL_PITCH_YAWRATE_THRUST, 50);

  control_msg_.roll = 0;
  control_msg_.pitch = 0;
  control_msg_.yaw_rate = 0;
  control_msg_.thrust.x = 0;
  control_msg_.thrust.y = 0;
  control_msg_.thrust.z = 0;
  current_yaw_vel_ = 0;

  pnh.param("axis_direction_roll", axes_.roll_direction, 1);
  pnh.param("axis_direction_pitch", axes_.pitch_direction, 1);
  pnh.param("axis_direction_thrust", axes_.thrust_direction, 1);

  pnh.param("max_v_xy", max_.v_xy, 0.8);  // [m/s]
  pnh.param("max_roll", max_.roll, 4.5 * M_PI / 180.0);  // [rad]
  pnh.param("max_pitch", max_.pitch, 4.5 * M_PI / 180.0);  // [rad]
  pnh.param("max_yaw_rate", max_.rate_yaw, 45.0 * M_PI / 180.0);  // [rad/s]
  pnh.param("max_thrust", max_.thrust, 1.58);  // [N]
  pnh.param("division_factor", div_factor, 85.0);  
  pnh.param("v_yaw_step", v_yaw_step_, 0.05);  // [rad/s]
  pnh.param("v_yaw_step", v_yaw_step_, 0.05);
  pnh.param("thrust_weight_offset_newtons",thrust_weight_offset_newtons_, 0.5870304);//14.896);
  edrone_sub_ = nh_.subscribe("/drone_command", 50, &Edrone_Trans::TransCallback, this);
  namespace_ = nh_.getNamespace();
}

void Edrone_Trans::StopMav() {
  control_msg_.roll = 0;
  control_msg_.pitch = 0;
  control_msg_.yaw_rate = 0;
  control_msg_.thrust.x = 0;
  control_msg_.thrust.y = 0;
  control_msg_.thrust.z = 0;
}

void Edrone_Trans::TransCallback(const edrone_client::edrone_msgs::ConstPtr& msg) {
  control_msg_.roll = ((msg->rcRoll-1500) * max_.roll/div_factor)*axes_.roll_direction;
  control_msg_.pitch = ((msg->rcPitch-1500) * max_.pitch/div_factor)* axes_.pitch_direction;
  control_msg_.yaw_rate = current_yaw_vel_;
  if(msg->rcThrottle>1499)
    control_msg_.thrust.z = ((msg->rcThrottle-1500)*(max_.thrust-thrust_weight_offset_newtons_)/500.0)+(thrust_weight_offset_newtons_);
  else
    control_msg_.thrust.z = (1+((msg->rcThrottle-1500)/500.0))*(thrust_weight_offset_newtons_);
  // control_msg_.thrust.z = ((msg->rcThrottle-1000)/1000.0)*max_.thrust;
  // control_msg_.thrust.z = ((msg->rcThrottle-1500) * max_.thrust / 200.0 * axes_.thrust_direction)+thrust_weight_offset_newtons_;
  ros::Time update_time = ros::Time::now();
  control_msg_.header.stamp = update_time;
  control_msg_.header.frame_id = "rotors_edrone_frame";
  Publish();
}

void Edrone_Trans::Publish() {
  ctrl_pub_.publish(control_msg_);
}
int main(int argc, char** argv) {
  ros::init(argc, argv, "rotors_edrone_interface");
  Edrone_Trans edrone_trans;
  ros::spin();
  return 0;
}
// int main(int argc, char** argv) {
//   ros::init(argc, argv, "rotors_edrone_interface");
//   Edrone_Trans edrone_trans;
//   ros::Rate loop_rate(30);
//   while(ros::ok())
//   {
//     edrone_trans.Publish();
//     ros::spinOnce();
//     loop_rate.sleep();
//   }
//   ROS_INFO("EXITEDadasdasdas");
//   return 0;
// }


/*
 * Copyright 2015 Fadri Furrer, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Michael Burri, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Mina Kamel, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Janosch Nikolic, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Markus Achtelik, ASL, ETH Zurich, Switzerland
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


// #include "rotors_edrone_interface/edrone_trans.h"

// #include <mav_msgs/default_topics.h>

// Edrone_Trans::Edrone_Trans() {
//   ros::NodeHandle nh;
//   ros::NodeHandle pnh("~");
//   ctrl_pub_ = nh_.advertise<mav_msgs::RollPitchYawrateThrust> (
//     mav_msgs::default_topics::COMMAND_ROLL_PITCH_YAWRATE_THRUST, 50);

//   control_msg_.roll = 0;
//   control_msg_.pitch = 0;
//   control_msg_.yaw_rate = 0;
//   control_msg_.thrust.x = 0;
//   control_msg_.thrust.y = 0;
//   control_msg_.thrust.z = 0;
//   current_yaw_vel_ = 0;

//   pnh.param("axis_direction_roll", axes_.roll_direction, 1);
//   pnh.param("axis_direction_pitch", axes_.pitch_direction, 1);
//   pnh.param("axis_direction_thrust", axes_.thrust_direction, 1);

//   pnh.param("max_v_xy", max_.v_xy, 1.0);  // [m/s]
//   pnh.param("max_roll", max_.roll, 10.0 * M_PI / 180.0);  // [rad]
//   pnh.param("max_pitch", max_.pitch, 10.0 * M_PI / 180.0);  // [rad]
//   pnh.param("max_yaw_rate", max_.rate_yaw, 45.0 * M_PI / 180.0);  // [rad/s]
//   pnh.param("max_thrust", max_.thrust, 30.0);  // [N]
//   pnh.param("thrust_weight_offset_newtons",thrust_weight_offset_newtons_, 14.896);
//   pnh.param("division_factor", div_factor, 85.0);  
//   pnh.param("v_yaw_step", v_yaw_step_, 0.05);  // [rad/s]
//   edrone_sub_ = nh_.subscribe("/drone_command", 50, &Edrone_Trans::TransCallback, this);
//   namespace_ = nh_.getNamespace();
// }

// void Edrone_Trans::StopMav() {
//   control_msg_.roll = 0;
//   control_msg_.pitch = 0;
//   control_msg_.yaw_rate = 0;
//   control_msg_.thrust.x = 0;
//   control_msg_.thrust.y = 0;
//   control_msg_.thrust.z = 0;
// }

// void Edrone_Trans::TransCallback(const edrone_client::edrone_msgs::ConstPtr& msg) {
//   control_msg_.roll = ((msg->rcRoll-1500) * max_.roll/div_factor)*axes_.roll_direction;
//   control_msg_.pitch = ((msg->rcPitch-1500) * max_.pitch/div_factor)* axes_.pitch_direction;
//   control_msg_.yaw_rate = current_yaw_vel_;
//   control_msg_.thrust.z = ((msg->rcThrottle-1500) * max_.thrust / 50.0 * axes_.thrust_direction)+thrust_weight_offset_newtons_;
//   ros::Time update_time = ros::Time::now();
//   control_msg_.header.stamp = update_time;
//   control_msg_.header.frame_id = "rotors_edrone_frame";
//   Publish();
// }

// void Edrone_Trans::Publish() {
//   ctrl_pub_.publish(control_msg_);
// }

// int main(int argc, char** argv) {
//   ros::init(argc, argv, "rotors_edrone_interface");
//   Edrone_Trans edrone_trans;
//   ros::spin();
//   return 0;
// }
