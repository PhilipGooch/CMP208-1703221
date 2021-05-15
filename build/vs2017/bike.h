#pragma once

#include <vector>

#include "skeleton.h"
#include "wheel.h"			

namespace gef
{
	class Mesh;
}
class b2World;

class Bike
{
public:
	//Passing the Bike it's initial position.
	Bike(float x, float y);
	~Bike();

	void Init(b2World* world, std::vector<gef::Mesh*> meshs);

	//Get objects.
	inline GameObject* GetBackWheel() { return &back_wheel_; }
	inline GameObject* GetBackSuspension() { return &back_suspension_; }
	inline GameObject* GetFrame() { return &frame_; }
	inline GameObject* GetFrontSuspension() { return &front_suspension_; }
	inline GameObject* GetFrontWheel() { return &front_wheel_; }
	inline GameObject* GetRiderLowerLeg() { return &rider_lower_leg_; }
	inline GameObject* GetRiderUpperLeg() { return &rider_upper_leg_; }
	inline GameObject* GetRiderBody() { return &rider_body_; }
	inline GameObject* GetRiderUpperArm() { return &rider_upper_arm_; }
	inline GameObject* GetRiderLowerArm() { return &rider_lower_arm_; }
	inline GameObject* GetRiderHead() { return &rider_head_; }

	//Get joints.
	inline b2RevoluteJoint* GetBackWheelJoint() { return back_wheel_joint_; }
	inline b2RevoluteJoint* GetBackSuspensionJoint() { return back_suspension_joint_; }
	inline b2PrismaticJoint* GetFrontSuspensionJoint() { return front_suspension_joint_; }
	inline b2RevoluteJoint* GetFrontWheelJoint() { return front_wheel_joint_; }
	inline b2RevoluteJoint* GetAnkleJoint() { return ankle_joint_; }
	inline b2RevoluteJoint* GetKneeJoint() { return knee_joint_; }
	inline b2RevoluteJoint* GetHipJoint() { return hip_joint_; }
	inline b2RevoluteJoint* GetShoulderJoint() { return shoulder_joint_; }
	inline b2RevoluteJoint* GetElbowJoint() { return elbow_joint_; }
	inline b2RevoluteJoint* GetWristJoint() { return wrist_joint_; }
	inline b2WeldJoint* GetNeckJoint() { return neck_joint_; }

	//Get joint angles.
	inline float GetHipAngle() { return hip_angle_; }
	inline float GetShoulderAngle() { return shoulder_angle_; }

	//Getters.
	inline bool GetCrashed() { return crashed_; };
	inline float GetMaxSpeed() { return max_speed_; };
	inline float GetAcceleration() { return acceleration_; };
	inline float GetFrontSuspensionForce() { return front_suspension_force_; }
	inline float GetAccelerating() { return accelerating_; }

	//Setters.
	inline void SetAccelerating( bool accelerating ) { accelerating_ = accelerating; }

	//Get booleans for if at max lean.
	inline bool GetAnkleMaxBack() { return ankle_max_back_; }
	inline bool GetAnkleMaxForward() { return ankle_max_forward_; }
	inline bool GetKneeMaxBack() { return knee_max_back_; }
	inline bool GetKneeMaxForward() { return knee_max_forward_; }
	inline bool GetHipMaxBack() { return hip_max_back_; }
	inline bool GetHipMaxForward() { return hip_max_forward_; }

	//Set booleans for if at max lean.
	inline void SetAnkleMaxBack(bool max_back) { ankle_max_back_ = max_back; }
	inline void SetAnkleMaxForward(bool max_forward) { ankle_max_forward_ = max_forward; }
	inline void SetKneeMaxBack(bool max_back) { knee_max_back_ = max_back; }
	inline void SetKneeMaxForward(bool max_forward) { knee_max_forward_ = max_forward; }
	inline void SetHipMaxBack(bool max_back) { hip_max_back_ = max_back; }
	inline void SetHipMaxForward(bool max_forward) { hip_max_forward_ = max_forward; }

	//Changes and destroys joints to make rider fall off bike.
	void FallOff();

private:
	b2World* world_;
	
	//World position of center of back wheel.
	float x_, y_;

	//Methods for applying motors and limits to revolute joints.
	void ApplyMotor(b2RevoluteJoint * joint, float max_torque, float speed);
	void ApplyLimits(b2RevoluteJoint * joint, float lower_limit, float upper_limit);

	//Methods for creating joints.
	b2RevoluteJoint* CreateRevoluteJoint(GameObject& body_a, GameObject& body_b, b2Vec2 anchor_a, b2Vec2 anchor_b);
	b2WeldJoint* CreateWeldJoint(GameObject& body_a, GameObject& body_b, b2Vec2 anchor_a, b2Vec2 anchor_b);
	b2PrismaticJoint* CreatePrismaticJoint(GameObject& body_a, GameObject& body_b, b2Vec2 anchor_a, b2Vec2 anchor_b, float reference_angle, b2Vec2 local_axis, float max_force);

	//Rider's limb proportions.
	float knee_height_;
	float hip_height_;
	float shoulder_height_;
	float elbow_height_;
	float wrist_height_;
	float head_height_;
	float scale_;

	//Rider's joint angles.
	float ankle_angle_;
	float knee_angle_;
	float hip_angle_;
	float shoulder_angle_;
	float elbow_angle_;
	float head_angle_;

	//Bike's movement variables.
	float speed_;
	float acceleration_;
	float max_speed_;
	bool crashed_;
	float front_suspension_force_;
	bool accelerating_;

	//Joint limits.
	bool ankle_max_forward_;
	bool ankle_max_back_;
	bool knee_max_forward_;
	bool knee_max_back_;
	bool hip_max_forward_;
	bool hip_max_back_;

	//Wheel radius used for spawn position of bike.
	float wheel_radius_;

	//Position of joints.
	b2Vec2 back_wheel_vertex_;
	b2Vec2 back_suspension_vertex_;
	b2Vec2 front_suspension_vertex_;
	b2Vec2 front_wheel_vertex_;
	b2Vec2 ankle_vertex_;
	b2Vec2 knee_vertex_;
	b2Vec2 hip_vertex_;
	b2Vec2 shoulder_vertex_;
	b2Vec2 elbow_vertex_;
	b2Vec2 wrist_vertex_;
	b2Vec2 head_vertex_;

	//Objects.
	Wheel back_wheel_;
	Skeleton back_suspension_;
	Skeleton frame_;
	Skeleton front_suspension_;
	Wheel front_wheel_;
	Skeleton rider_lower_leg_;
	Skeleton rider_upper_leg_;
	Skeleton rider_body_;
	Skeleton rider_upper_arm_;
	Skeleton rider_lower_arm_;
	Skeleton rider_head_;

	//Joints.
	b2RevoluteJoint* back_wheel_joint_;
	b2RevoluteJoint* back_suspension_joint_;
	b2PrismaticJoint* front_suspension_joint_;
	b2RevoluteJoint* front_wheel_joint_;
	b2RevoluteJoint* ankle_joint_;
	b2RevoluteJoint* knee_joint_;
	b2RevoluteJoint* hip_joint_;
	b2RevoluteJoint* shoulder_joint_;
	b2RevoluteJoint* elbow_joint_;
	b2RevoluteJoint* wrist_joint_;
	b2WeldJoint* neck_joint_;

	//Center positions of bodys.
	b2Vec2 back_suspension_origin_;
	b2Vec2 frame_origin_;
	b2Vec2 front_suspension_origin_;
	b2Vec2 rider_body_origin_;
	b2Vec2 rider_lower_leg_origin_;
	b2Vec2 rider_upper_leg_origin_;
	b2Vec2 rider_upper_arm_origin_;
	b2Vec2 rider_lower_arm_origin_;
	b2Vec2 rider_head_origin_;

	//Angles of joints.
	float back_suspension_angle_;
	float frame_angle_;
	float front_suspension_angle_;
	float rider_lower_leg_angle_;
	float rider_upper_leg_angle_;
	float rider_body_angle_;
	float rider_upper_arm_angle_;
	float rider_lower_arm_angle_;
	float rider_head_angle_;

	//Lengths of bodys.
	float back_suspension_length_;
	float frame_length_;
	float front_suspension_length_;
	float rider_lower_leg_length_;
	float rider_upper_leg_length_;
	float rider_body_length_;
	float rider_upper_arm_length_;
	float rider_lower_arm_length_;
	float rider_head_length_;

	//Relative angles of joints.
	float back_suspension_joint_angle_;
	float front_suspension_joint_angle_;
	float rider_lower_leg_joint_angle_;
	float rider_upper_leg_joint_angle_;

};

