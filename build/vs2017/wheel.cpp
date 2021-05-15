#include "wheel.h"

Wheel::Wheel()
{
}

Wheel::~Wheel()
{
}

void Wheel::Init(b2World* world, b2Vec2* origin, float radius, b2BodyType type, gef::Mesh* mesh, float density, OBJECT_TYPE object_type)
{
	//Set the type.
	SetType(object_type);

	//Setup the mesh.
	set_mesh(mesh);

	//Setup the body.
	b2BodyDef body_def;
	body_def.type = b2_dynamicBody;
	body_def.position = b2Vec2(origin->x, origin->y);
	body_ = world->CreateBody(&body_def);

	//Setup the shape.
	b2CircleShape shape;
	shape.m_radius = radius;

	//Setup the fixture.
	b2FixtureDef fixture_def;
	fixture_def.shape = &shape;
	fixture_def.density = density;
	fixture_def.restitution = 0.0f;
	fixture_def.friction = 1.7f;

	//Stating what the fixture can interact with.
	fixture_def.filter.categoryBits = WHEEL_CATAGORY;
	fixture_def.filter.maskBits = WHEEL_CATAGORY | DEFAULT_CATAGORY | RIDER_CATAGORY;

	//Create the fixture on the rigid body
	body_->CreateFixture(&fixture_def);

	//Update visuals from simulation data
	UpdateFromSimulation();

	//Create a connection between the rigid body and GameObject
	body_->SetUserData(this);
}
