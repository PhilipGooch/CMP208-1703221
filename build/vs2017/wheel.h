#pragma once

#include "game_object.h"

class Wheel : public GameObject
{
public:
	Wheel();
	~Wheel();

	void Init(b2World* world, b2Vec2* origin, float radius,  b2BodyType type, gef::Mesh* mesh, float density, OBJECT_TYPE object_type);
};