#include "skeleton.h"

Skeleton::Skeleton()
{
}


Skeleton::~Skeleton()
{
}

void Skeleton::Init(b2World* world, b2Vec2* origin, float angle, float length, b2BodyType body_type, float z, OBJECT_TYPE object_type, float density, gef::Mesh* mesh)
{
	//Set the type.
	SetType(object_type);

	//Setup the mesh.
	set_mesh(mesh);
	
	//Setup the body.
	b2BodyDef body_def;
	body_def.type = b2_dynamicBody;
	body_def.position = b2Vec2(origin->x, origin->y);
	body_def.angle = angle;
	body_ = world->CreateBody(&body_def);

	//Setup the shape.
	b2PolygonShape shape;
	shape.SetAsBox(length / 2, 0.1f, b2Vec2(0, 0), 0);	

	//Setup the fixture.
	b2FixtureDef fixture_def;
	fixture_def.shape = &shape;
	fixture_def.density = density;

	//Stating what the fixture can interact with.
	if (object_type == FRAME)
	{ 
		fixture_def.filter.categoryBits = DEFAULT_CATAGORY;
		fixture_def.filter.maskBits = DEFAULT_CATAGORY | WHEEL_CATAGORY | SUSPENSION_CATAGORY | RIDER_CATAGORY;
	}
	else if (object_type == RIDER_LOWER_LEG || 
			 object_type == RIDER_UPPER_LEG || 
			 object_type == RIDER_BODY || 
			 object_type == RIDER_UPPER_ARM || 
			 object_type == RIDER_LOWER_ARM || 
			 object_type == RIDER_HEAD )
	{
		fixture_def.filter.categoryBits = RIDER_CATAGORY;
		fixture_def.filter.maskBits = DEFAULT_CATAGORY ;
	}
	else if (object_type == BACK_SUSPENSION ||
			 object_type == FRONT_SUSPENSION)
	{
		fixture_def.filter.categoryBits = SUSPENSION_CATAGORY;
		fixture_def.filter.maskBits = DEFAULT_CATAGORY;
	}

	//Create the fixture on the rigid body
	body_->CreateFixture(&fixture_def);

	//Update visuals from simulation data
	UpdateFromSimulation();

	//Create a connection between the rigid body and GameObject
	body_->SetUserData(this);
}
