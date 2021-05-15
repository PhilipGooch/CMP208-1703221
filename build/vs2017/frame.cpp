#include "frame.h"



Frame::Frame()
{
}


Frame::~Frame()
{
}

void Frame::Init(PrimitiveBuilder* primitive_builder, b2World* world, b2Vec2* origin, float angle_1, float angle_2, float length, b2BodyType type, float z, OBJECT_TYPE object_type)
{
	SetType(object_type);

	// setup the mesh
	set_mesh(primitive_builder->CreateBoxMesh(gef::Vector4(length / 2, 0.1, 0.1), gef::Vector4(0, 0, z))); // leave center as 0!

	// body
	b2BodyDef body_def;
	body_def.type = b2_dynamicBody;
	body_def.position = b2Vec2(origin->x, origin->y);
	body_def.angle = 0;
	body_ = world->CreateBody(&body_def);

	// create the shape
	b2PolygonShape shape;
	shape.SetAsBox(length / 2, 0.1, b2Vec2(0, 0), 0);	// leave angle and center as 0!

														// create the fixture
	b2FixtureDef fixture_def;
	fixture_def.shape = &shape;
	fixture_def.density = 9.905f;
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
		fixture_def.filter.maskBits = 0;
	}
	else if (object_type == BACK_SUSPENSION ||
			 object_type == FRONT_SUSPENSION)
	{
		fixture_def.filter.categoryBits = SUSPENSION_CATAGORY;
		fixture_def.filter.maskBits = DEFAULT_CATAGORY;
	}

	// create the fixture on the rigid body
	body_->CreateFixture(&fixture_def);

	// update visuals from simulation data
	UpdateFromSimulation();

	// create a connection between the rigid body and GameObject
	body_->SetUserData(this);
}

void Frame::CollisionResponse()
{
}
