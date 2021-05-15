#include "bike.h"

Bike::Bike(float x, float y) :
	x_(x),
	y_(y)
{
}

Bike::~Bike()
{
}

void Bike::Init(b2World* world, std::vector<gef::Mesh*> meshs)
{	
	world_ = world;

	//Setting rider's limb proportions.
	knee_height_ = 2.5f;
	hip_height_ = 2.2f;
	shoulder_height_ = 2.5f;
	elbow_height_ = 1.75f;
	wrist_height_ = 1.75f;
	head_height_ = 2.5f;

	//Setting a scaler for the size of the rider.
	scale_ = 0.85f;

	//Setting the starting angles for all the rider's joints.
	ankle_angle_ = 1.45f;
	knee_angle_ = 1.45f;
	hip_angle_ = -1.7f;
	shoulder_angle_ = -1.5f;
	elbow_angle_ = 0.0f;
	head_angle_ = 0.0f;

	//Setting bike's movement variables.
	speed_ = 0.0f;
	acceleration_ = 26.0f;
	max_speed_ = 27.0f;
	accelerating_ = false;

	//Setting the strength of the front suspension.
	front_suspension_force_ = 140.0f;

	//Setting the radius of the wheel.
	wheel_radius_ = 1.67202f;

	//Setting booleans for if specific joints have reached their limits.
	ankle_max_forward_ = false;
	ankle_max_back_ = false;
	knee_max_forward_ = false;
	knee_max_back_ = false;
	hip_max_forward_ = false;
	hip_max_back_ = false;

	//Boolean for if the rider has fallen off or not.
	crashed_ = false;

	//Setting positions for the four joints of the bike.
	back_wheel_vertex_ = b2Vec2(x_, y_ + wheel_radius_);

	back_suspension_vertex_ = b2Vec2(x_ + 2.35624f, y_ + 1.13231f + wheel_radius_);

	front_suspension_vertex_ = b2Vec2(x_ + 5.36796f, y_ + 2.0607f + wheel_radius_);

	front_wheel_vertex_ = b2Vec2(x_ + 6.16922f, y_ + 0.0f + wheel_radius_);

	//The rest of the values that are needed are calculated using the pre-defined, hard coded variables above.

	//Calculating the positions of the rider's joint positions.
	ankle_vertex_    = b2Vec2(back_suspension_vertex_.x + 0.1f, 
						      back_suspension_vertex_.y - 1.0f);

	knee_vertex_     = b2Vec2(ankle_vertex_.x + cos(ankle_angle_) * knee_height_ * scale_, 
						      ankle_vertex_.y + sin(ankle_angle_) * knee_height_ * scale_);

	hip_vertex_      = b2Vec2(knee_vertex_.x + cos(ankle_angle_ + knee_angle_) * hip_height_ * scale_, 
						      knee_vertex_.y + sin(ankle_angle_ + knee_angle_) * hip_height_ * scale_);

	shoulder_vertex_ = b2Vec2(hip_vertex_.x + cos(ankle_angle_ + knee_angle_ + hip_angle_) * shoulder_height_ * scale_, 
							  hip_vertex_.y + sin(ankle_angle_ + knee_angle_ + hip_angle_) * shoulder_height_ * scale_);

	elbow_vertex_    = b2Vec2(shoulder_vertex_.x + cos(ankle_angle_ + knee_angle_ + hip_angle_ + shoulder_angle_) * elbow_height_ * scale_, 
						      shoulder_vertex_.y + sin(ankle_angle_ + knee_angle_ + hip_angle_ + shoulder_angle_) * elbow_height_ * scale_);

	wrist_vertex_    = b2Vec2(elbow_vertex_.x + cos(ankle_angle_ + knee_angle_ + hip_angle_ + shoulder_angle_ + elbow_angle_) * wrist_height_ * scale_, 
							  elbow_vertex_.y + sin(ankle_angle_ + knee_angle_ + hip_angle_ + shoulder_angle_ + elbow_angle_) * wrist_height_ * scale_);

	head_vertex_     = b2Vec2(shoulder_vertex_.x + cos(ankle_angle_ + knee_angle_ + hip_angle_ + head_angle_) * head_height_ * scale_, 
							  shoulder_vertex_.y + sin(ankle_angle_ + knee_angle_ + hip_angle_ + head_angle_) * head_height_ * scale_);

	//Calculating the center positions of the bodys.
	back_suspension_origin_  = b2Vec2(back_wheel_vertex_.x + (back_suspension_vertex_.x - back_wheel_vertex_.x) / 2, 
									  back_wheel_vertex_.y + (back_suspension_vertex_.y - back_wheel_vertex_.y) / 2);

	frame_origin_			 = b2Vec2(back_suspension_vertex_.x + (front_suspension_vertex_.x - back_suspension_vertex_.x) / 2, 
									  back_suspension_vertex_.y + (front_suspension_vertex_.y - back_suspension_vertex_.y) / 2);

	front_suspension_origin_ = b2Vec2(front_suspension_vertex_.x + (front_wheel_vertex_.x - front_suspension_vertex_.x) / 2, 
									  front_suspension_vertex_.y + (front_wheel_vertex_.y - front_suspension_vertex_.y) / 2);

	rider_body_origin_		 = b2Vec2(hip_vertex_.x + (shoulder_vertex_.x - hip_vertex_.x) / 2, 
									  hip_vertex_.y + (shoulder_vertex_.y - hip_vertex_.y) / 2);

	rider_lower_leg_origin_  = b2Vec2(ankle_vertex_.x + (knee_vertex_.x - ankle_vertex_.x) / 2, 
									  ankle_vertex_.y + (knee_vertex_.y - ankle_vertex_.y) / 2);

	rider_upper_leg_origin_  = b2Vec2(knee_vertex_.x + (hip_vertex_.x - knee_vertex_.x) / 2, 
									  knee_vertex_.y + (hip_vertex_.y - knee_vertex_.y) / 2);

	rider_upper_arm_origin_  = b2Vec2(shoulder_vertex_.x + (elbow_vertex_.x - shoulder_vertex_.x) / 2, 
									  shoulder_vertex_.y + (elbow_vertex_.y - shoulder_vertex_.y) / 2);

	rider_lower_arm_origin_  = b2Vec2(elbow_vertex_.x + (wrist_vertex_.x - elbow_vertex_.x) / 2, 
									  elbow_vertex_.y + (wrist_vertex_.y - elbow_vertex_.y) / 2);

	rider_head_origin_		 = b2Vec2(shoulder_vertex_.x + (head_vertex_.x - shoulder_vertex_.x) / 2, 
									  shoulder_vertex_.y + (head_vertex_.y - shoulder_vertex_.y) / 2);

	//Calculating the angles of the bodys.
	back_suspension_angle_  = atan2(back_suspension_vertex_.y - back_wheel_vertex_.y, 
									back_suspension_vertex_.x - back_wheel_vertex_.x);

	frame_angle_			= atan2(front_suspension_vertex_.y - back_suspension_vertex_.y, 
									front_suspension_vertex_.x - back_suspension_vertex_.x);

	front_suspension_angle_ = atan2(front_wheel_vertex_.y - front_suspension_vertex_.y, 
									front_wheel_vertex_.x - front_suspension_vertex_.x);

	rider_lower_leg_angle_  = atan2(ankle_vertex_.y - knee_vertex_.y, 
									ankle_vertex_.x - knee_vertex_.x);

	rider_upper_leg_angle_  = atan2(knee_vertex_.y - hip_vertex_.y, 
									knee_vertex_.x - hip_vertex_.x);

	rider_body_angle_		= atan2(hip_vertex_.y - shoulder_vertex_.y, 
									hip_vertex_.x - shoulder_vertex_.x);

	rider_upper_arm_angle_  = atan2(shoulder_vertex_.y - elbow_vertex_.y, 
									shoulder_vertex_.x - elbow_vertex_.x);

	rider_lower_arm_angle_  = atan2(elbow_vertex_.y - wrist_vertex_.y, 
									elbow_vertex_.x - wrist_vertex_.x);

	rider_head_angle_		= atan2(shoulder_vertex_.y - head_vertex_.y, 
									shoulder_vertex_.x - head_vertex_.x);

	//Calculating the lengths of the bodys.
	back_suspension_length_  = sqrt((back_suspension_vertex_.x - back_wheel_vertex_.x) * (back_suspension_vertex_.x - back_wheel_vertex_.x) + 
									(back_suspension_vertex_.y - back_wheel_vertex_.y) * (back_suspension_vertex_.y - back_wheel_vertex_.y));

	frame_length_			 = sqrt((front_suspension_vertex_.x - back_suspension_vertex_.x) * (front_suspension_vertex_.x - back_suspension_vertex_.x) + 
									(front_suspension_vertex_.y - back_suspension_vertex_.y) * (front_suspension_vertex_.y - back_suspension_vertex_.y));

	front_suspension_length_ = sqrt((front_wheel_vertex_.x - front_suspension_vertex_.x) * (front_wheel_vertex_.x - front_suspension_vertex_.x) + 
									(front_wheel_vertex_.y - front_suspension_vertex_.y) * (front_wheel_vertex_.y - front_suspension_vertex_.y));

	rider_lower_leg_length_  = sqrt((ankle_vertex_.x - knee_vertex_.x) * (ankle_vertex_.x - knee_vertex_.x) + 
									(ankle_vertex_.y - knee_vertex_.y) * (ankle_vertex_.y - knee_vertex_.y));

	rider_upper_leg_length_  = sqrt((knee_vertex_.x - hip_vertex_.x) * (knee_vertex_.x - hip_vertex_.x) + 
									(knee_vertex_.y - hip_vertex_.y) * (knee_vertex_.y - hip_vertex_.y));

	rider_body_length_		 = sqrt((hip_vertex_.x - shoulder_vertex_.x) * (hip_vertex_.x - shoulder_vertex_.x) + 
									(hip_vertex_.y - shoulder_vertex_.y) * (hip_vertex_.y - shoulder_vertex_.y));

	rider_upper_arm_length_  = sqrt((shoulder_vertex_.x - elbow_vertex_.x) * (shoulder_vertex_.x - elbow_vertex_.x) + 
									(shoulder_vertex_.y - elbow_vertex_.y) * (shoulder_vertex_.y - elbow_vertex_.y));

	rider_lower_arm_length_  = sqrt((elbow_vertex_.x - wrist_vertex_.x) * (elbow_vertex_.x - wrist_vertex_.x) + 
									(elbow_vertex_.y - wrist_vertex_.y) * (elbow_vertex_.y - wrist_vertex_.y));

	rider_head_length_		 = 1;

	//Calculating the angles between the bodys.
	back_suspension_joint_angle_ = frame_angle_ - back_suspension_angle_;

	front_suspension_joint_angle_ = front_suspension_angle_ - frame_angle_;

	rider_lower_leg_joint_angle_ = rider_lower_leg_angle_ - frame_angle_;

	rider_upper_leg_joint_angle_ = rider_upper_leg_angle_ - rider_lower_leg_angle_;

	//Initializing the bike's objects.
	back_wheel_.Init(world, &back_wheel_vertex_, wheel_radius_, b2_dynamicBody, meshs[0], 1.0f, BACK_WHEEL);

	back_suspension_.Init(world, &back_suspension_origin_, back_suspension_angle_, back_suspension_length_, b2_dynamicBody, 0.0f, BACK_SUSPENSION, 5, meshs[2]);

	frame_.Init(world, &frame_origin_, frame_angle_, frame_length_, b2_dynamicBody, 0.0, FRAME, 5.0f, meshs[3]);

	front_suspension_.Init(world, &front_suspension_origin_, front_suspension_angle_, front_suspension_length_, b2_dynamicBody, 0.0f, FRONT_SUSPENSION, 5, meshs[1]);

	front_wheel_.Init(world, &front_wheel_vertex_, wheel_radius_, b2_dynamicBody, meshs[0], 1.2f, FRONT_WHEEL);

	rider_lower_leg_.Init(world, &rider_lower_leg_origin_, rider_lower_leg_angle_, rider_lower_leg_length_, b2_dynamicBody, 0.5f, RIDER_LOWER_LEG, 3, meshs[4]);

	rider_upper_leg_.Init(world, &rider_upper_leg_origin_, rider_upper_leg_angle_, rider_upper_leg_length_, b2_dynamicBody, 0.5f, RIDER_UPPER_LEG, 3, meshs[5]);

	rider_body_.Init(world, &rider_body_origin_, rider_body_angle_, rider_body_length_, b2_dynamicBody, 0.5f, RIDER_BODY, 6, meshs[6]);

	rider_upper_arm_.Init(world, &rider_upper_arm_origin_, rider_upper_arm_angle_, rider_upper_arm_length_, b2_dynamicBody, 0.5f, RIDER_UPPER_ARM, 1, meshs[7]);
																 
	rider_lower_arm_.Init(world, &rider_lower_arm_origin_, rider_lower_arm_angle_, rider_lower_arm_length_, b2_dynamicBody, 0.5f, RIDER_LOWER_ARM, 1, meshs[8]);

	rider_head_.Init(world, &rider_head_origin_, head_angle_, rider_head_length_, b2_dynamicBody, 0.5f, RIDER_HEAD, 1, meshs[9]);

	//Creating revolute joints.
	back_wheel_joint_ = CreateRevoluteJoint(back_wheel_, back_suspension_, b2Vec2(0, 0), b2Vec2(-back_suspension_length_ / 2, 0));

	back_suspension_joint_ = CreateRevoluteJoint(back_suspension_, frame_, b2Vec2(back_suspension_length_ / 2, 0), b2Vec2(-frame_length_ / 2, 0));

	front_wheel_joint_ = CreateRevoluteJoint(front_suspension_, front_wheel_, b2Vec2(front_suspension_length_ / 2, 0.0f), b2Vec2(0, 0));

	ankle_joint_ = CreateRevoluteJoint(frame_, rider_lower_leg_, b2Vec2(-frame_length_ / 2, -1.2f), b2Vec2(rider_lower_leg_length_ / 2, 0));

	knee_joint_ = CreateRevoluteJoint(rider_lower_leg_, rider_upper_leg_, b2Vec2(-rider_lower_leg_length_ / 2, 0), b2Vec2(rider_upper_leg_length_ / 2, 0));

	hip_joint_ = CreateRevoluteJoint(rider_upper_leg_, rider_body_, b2Vec2(-rider_upper_leg_length_ / 2, 0), b2Vec2(rider_body_length_ / 2, 0));

	shoulder_joint_ = CreateRevoluteJoint(rider_body_, rider_upper_arm_, b2Vec2(-rider_body_length_ / 2, 0), b2Vec2(rider_upper_arm_length_ / 2, 0));
	
	elbow_joint_ = CreateRevoluteJoint(rider_upper_arm_, rider_lower_arm_, b2Vec2(-rider_upper_arm_length_ / 2, 0), b2Vec2(rider_lower_arm_length_ / 2, 0));

	wrist_joint_ = CreateRevoluteJoint(rider_lower_arm_, frame_, b2Vec2(-rider_lower_arm_length_ / 2, 0), b2Vec2(-frame_length_ / 2 + 3, 2));

	//Creating weld joints.
	neck_joint_ = CreateWeldJoint(rider_body_, rider_head_, b2Vec2(-rider_body_length_ / 2, 0), b2Vec2(rider_head_length_, 0));

	//Creating prismatic joints.
	front_suspension_joint_ = CreatePrismaticJoint(frame_, front_suspension_, b2Vec2(frame_length_ / 2, 0.0f), b2Vec2(-front_suspension_length_ / 2, 0.0f), 
												   front_suspension_joint_angle_, b2Vec2(cos(front_suspension_joint_angle_), sin(front_suspension_joint_angle_)), 700.0f);

	//Applying motors to joints.
	ApplyMotor(back_wheel_joint_, 2000.0f, 0.0f);
	ApplyMotor(back_suspension_joint_, 650.f, -10.f);
	ApplyMotor(front_wheel_joint_, 2000.0f, 0.0f);
	ApplyMotor(ankle_joint_, 2000.0f, 1.0f);
	ApplyMotor(knee_joint_, 2000.0f, 1.0f);
	ApplyMotor(hip_joint_, 2000.0f, -1.0f);

	//Applying limits to joints.
	ApplyLimits(back_suspension_joint_, back_suspension_joint_angle_, back_suspension_joint_angle_ + 0.15f);
	ApplyLimits(ankle_joint_, rider_lower_leg_joint_angle_ - 0.1f, rider_lower_leg_joint_angle_);
	ApplyLimits(knee_joint_, 0, knee_angle_);
	ApplyLimits(hip_joint_, hip_angle_, hip_angle_ + 1.6f);
	ApplyLimits(shoulder_joint_, 3.1415f * 2 + shoulder_angle_ - 2, 3.1415f * 2 + shoulder_angle_);
	ApplyLimits(elbow_joint_, elbow_angle_, elbow_angle_ + 3.1415f / 2);
}

void Bike::ApplyMotor(b2RevoluteJoint* joint, float max_torque, float speed)
{
	joint->EnableMotor(true);
	joint->SetMaxMotorTorque(max_torque);
	joint->SetMotorSpeed(speed);
}

void Bike::ApplyLimits(b2RevoluteJoint* joint, float lower_limit, float upper_limit)
{
	joint->EnableLimit(true);
	joint->SetLimits(lower_limit, upper_limit);
}

b2RevoluteJoint* Bike::CreateRevoluteJoint(GameObject& body_a, GameObject& body_b, b2Vec2 anchor_a, b2Vec2 anchor_b)
{
	b2RevoluteJointDef revolute_joint_def;
	revolute_joint_def.collideConnected = false;
	revolute_joint_def.enableMotor = false;
	revolute_joint_def.enableLimit = false;
	revolute_joint_def.bodyA = body_a.GetBody();
	revolute_joint_def.bodyB = body_b.GetBody();
	revolute_joint_def.localAnchorA.Set(anchor_a.x, anchor_a.y);
	revolute_joint_def.localAnchorB.Set(anchor_b.x, anchor_b.y);
	return (b2RevoluteJoint*)world_->CreateJoint(&revolute_joint_def);
}

b2WeldJoint* Bike::CreateWeldJoint(GameObject& body_a, GameObject& body_b, b2Vec2 anchor_a, b2Vec2 anchor_b)
{
	b2WeldJointDef weld_joint_def;
	weld_joint_def.collideConnected = false;
	weld_joint_def.bodyA = body_a.GetBody();
	weld_joint_def.bodyB = body_b.GetBody();
	weld_joint_def.localAnchorA.Set(anchor_a.x, anchor_a.y);
	weld_joint_def.localAnchorB.Set(anchor_b.x, anchor_b.y);
	return (b2WeldJoint*)world_->CreateJoint(&weld_joint_def);
}

b2PrismaticJoint* Bike::CreatePrismaticJoint(GameObject& body_a, GameObject& body_b, b2Vec2 anchor_a, b2Vec2 anchor_b, float reference_angle, b2Vec2 local_axis, float max_force)
{
	b2PrismaticJointDef prismatic_joint_def;
	prismatic_joint_def.collideConnected = false;
	prismatic_joint_def.enableLimit = false;
	prismatic_joint_def.enableMotor = true;
	prismatic_joint_def.maxMotorForce = max_force;
	prismatic_joint_def.bodyA = body_a.GetBody();
	prismatic_joint_def.bodyB = body_b.GetBody();
	prismatic_joint_def.localAnchorA.Set(anchor_a.x, anchor_a.y);
	prismatic_joint_def.localAnchorB.Set(anchor_b.x, anchor_b.y);
	prismatic_joint_def.referenceAngle = reference_angle;
	prismatic_joint_def.localAxisA = local_axis;
	return (b2PrismaticJoint*)world_->CreateJoint(&prismatic_joint_def);
}

void Bike::FallOff()
{
	crashed_ = true;

	//Disabling motors to create ragdoll effect.
	back_wheel_joint_->EnableMotor(false);
	front_wheel_joint_->EnableMotor(false);
	ankle_joint_->EnableMotor(false);
	knee_joint_->EnableMotor(false);
	hip_joint_->EnableMotor(false);

	//Altering some joint limits to get desired ragdoll effect.
	hip_joint_->SetLimits(hip_angle_ + 0.15f, hip_angle_ + 1.6f);
	shoulder_joint_->SetLimits(3.1415f * 2 + shoulder_angle_ - 2, 3.1415f * 2);

	//Destroying ankle and wrist joints.
	world_->DestroyJoint(ankle_joint_);
	world_->DestroyJoint(wrist_joint_);
	ankle_joint_ = nullptr;
	wrist_joint_ = nullptr;
}