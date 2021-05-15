#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <graphics/mesh_instance.h>

//I only have pointers to box2d objects in this class so i could forward declare it. However it is small so i could do it just once here? Is this better than including it in all child class' cpp files?
#include <box2d/Box2D.h>				

//Bitmasks for collision detection.
#define DEFAULT_CATAGORY		1		//0001
#define SUSPENSION_CATAGORY		2		//0010
#define WHEEL_CATAGORY			4		//0100
#define RIDER_CATAGORY			8		//1000

//Types the GameObject can be.
enum OBJECT_TYPE
{
	ENVIRONMENT,
	BACK_WHEEL,
	BACK_SUSPENSION,
	FRAME,
	FRONT_SUSPENSION,
	FRONT_WHEEL,
	RIDER_LOWER_LEG,
	RIDER_UPPER_LEG,
	RIDER_BODY,
	RIDER_UPPER_ARM,
	RIDER_LOWER_ARM,
	RIDER_HEAD
};

class GameObject : public gef::MeshInstance
{
public:
	GameObject();

	//Deconstuctor must be a pure virtual method to be sure to be calling the correct deconstructor.
	virtual ~GameObject() = 0;

	//Updates the transform of this object from a physics rigid body.
	void UpdateFromSimulation();

	//virtual void Init() {}; //Child classes of GameObject have different parameters in their Init() functions. Is it ok for base class not to have an Init() function?

	//Getters.
	inline OBJECT_TYPE GetType() { return type_; }
	inline b2Body* GetBody() { return body_; }

	//Setters.
	inline void SetType(OBJECT_TYPE type) { type_ = type; }

protected:
	OBJECT_TYPE type_;
	b2Body* body_;
	gef::Mesh* mesh_;
};

#endif // _GAME_OBJECT_H