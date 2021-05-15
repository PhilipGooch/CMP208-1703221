#pragma once

#include "game_object.h"

class Bar : public GameObject
{
public:
	Bar();
	~Bar();

	void Init(b2World* world, gef::Mesh* meshs, b2Vec2 origin, const float radius);
};
