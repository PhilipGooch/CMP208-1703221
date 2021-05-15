#pragma once

#include "game_object.h"

class Frame : public GameObject
{
public:
	Frame();
	~Frame();

	void Init(PrimitiveBuilder* primitive_builder, b2World* world, b2Vec2* origin, float angle_1, float angle_2, float length, b2BodyType type, float z, OBJECT_TYPE object_type);
	void CollisionResponse();
};

