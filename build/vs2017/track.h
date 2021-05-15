#pragma once

#include "game_object.h"

class Track : public GameObject
{
public:
	Track();
	~Track();

	void Init(b2World* world, gef::Mesh* meshs, b2Vec2 origin, const b2Vec2* vertices, int num_vertices);
};

