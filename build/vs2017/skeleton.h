#pragma once

#include "game_object.h"

class Skeleton : public GameObject
{
public:
	Skeleton();
	~Skeleton();

	void Init(b2World* world, b2Vec2* origin, float angle, float length, b2BodyType type, float z, OBJECT_TYPE object_type, float density, gef::Mesh* mesh);
};

