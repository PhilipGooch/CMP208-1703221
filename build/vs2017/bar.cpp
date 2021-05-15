#include "bar.h"

Bar::Bar()
{
}


Bar::~Bar()
{
}

void Bar::Init(b2World* world, gef::Mesh* mesh, b2Vec2 origin, const float radius)
{
	//Set the type.
	SetType(ENVIRONMENT);

	//Setup the mesh.
	set_mesh(mesh);

	//Setup the body.
	b2BodyDef body_def;
	body_def.type = b2_staticBody;
	body_def.position = b2Vec2(origin.x, origin.y);
	body_ = world->CreateBody(&body_def);

	//Setup the shape.
	b2CircleShape shape;
	shape.m_radius = radius;

	//Setup the fixture.
	b2FixtureDef fixture_def;
	fixture_def.shape = &shape;

	//Stating what the fixture can interact with.
	fixture_def.filter.categoryBits = DEFAULT_CATAGORY;
	fixture_def.filter.maskBits = DEFAULT_CATAGORY | WHEEL_CATAGORY | SUSPENSION_CATAGORY | RIDER_CATAGORY;
	fixture_def.friction = 1;

	//Create the fixture on the rigid body
	body_->CreateFixture(&fixture_def);

	//Update visuals from simulation data
	UpdateFromSimulation();

	//Create a connection between the rigid body and GameObject
	body_->SetUserData(this);
}

