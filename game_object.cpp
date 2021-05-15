#include "game_object.h"

GameObject::GameObject() :
	body_(NULL)
{
}

GameObject::~GameObject()
{
}

void GameObject::UpdateFromSimulation()
{
	if (body_)
	{
		// setup object rotation
		gef::Matrix44 object_rotation;
		object_rotation.RotationZ(body_->GetAngle());

		// setup the object translation
		gef::Vector4 object_translation(body_->GetPosition().x, body_->GetPosition().y, 0.0f);

		// build object transformation matrix
		gef::Matrix44 object_transform = object_rotation;
		object_transform.SetTranslation(object_translation);
		set_transform(object_transform);
	}
}