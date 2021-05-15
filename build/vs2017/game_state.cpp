#include "game_state.h"

#include "primitive_builder.h"
#include <input/input_manager.h>
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/renderer_3d.h>
#include <maths/math_utils.h>
#include <graphics/mesh.h>
#include <input/sony_controller_input_manager.h>
#include <istream>
#include <system/file.h>
#include <audio/audio_manager.h>
#include <sstream>

#include "bike.h"
#include "track.h"
#include "bar.h"
#include "state_handler.h"
#include "graphics/scene.h"
#include "state_handler.h"

#include <system/debug_log.h>

GameState::GameState() :
	world_(NULL),
	renderer_3d_(NULL),
	primitive_builder_(NULL),
	angle_(3.1415f / 2),
	zoom_(20)
{
}

void GameState::Init(gef::Platform* platform, gef::InputManager* input_manager, gef::SpriteRenderer* sprite_renderer, gef::AudioManager* audio_manager, StateHandler* state_handler, int level, int sfx_id)
{
	//Setting the type of state.
	type_ = GAME;

	//Setting member variables to the variables passed in.
	platform_ = platform;
	input_manager_ = input_manager;
	sprite_renderer_ = sprite_renderer;		
	audio_manager_ = audio_manager;
	state_handler_ = state_handler;

	level_ = level;
	crash_time_ = 3.f;
	crash_timer_ = 0.f,
	track_asset_instances_.clear();
	track_assets_.clear();
	chain_shape_vertices_.clear();

	//Initializing renderer_3d_.
	renderer_3d_ = gef::Renderer3D::Create(*platform_);

	//Initializing primitive_builder_.
	primitive_builder_ = new PrimitiveBuilder(*platform_);

	//Setting gravity vector for world.
	b2Vec2 gravity(0.0f, -9.81f * 2.5f);

	//Initializing world.
	world_ = new b2World(gravity);

	//Creating a list of flie names to use when loading models.
	bike_filenames_.push_back("wheel.scn");
	bike_filenames_.push_back("front_suspension.scn");
	bike_filenames_.push_back("back_suspension.scn");
	bike_filenames_.push_back("frame.scn");
	bike_filenames_.push_back("lower_leg.scn");
	bike_filenames_.push_back("upper_leg.scn");
	bike_filenames_.push_back("body.scn");
	bike_filenames_.push_back("upper_arm.scn");
	bike_filenames_.push_back("lower_arm.scn");
	bike_filenames_.push_back("helmet.scn");

	//Loading models.
	for (int i = 0; i < (int)bike_filenames_.size(); i++)
	{
		const char* bike_wheel_filename = bike_filenames_[i].c_str();
		bike_asset_instances_.push_back(LoadSceneAsset(*platform, bike_wheel_filename));
		bike_assets_.push_back(GetMeshFromSceneAssets(bike_asset_instances_[bike_asset_instances_.size() - 1]));
	}

	//Reseting crash timer.
	crash_timer_ = 0;

	//Initializing bike.
	//1.67202 = wheel radius.
	//0.08876 = y position of track start.
	if(level_ == 1)
		bike_ = new Bike(1, 0.f);
	else if (level_ == 2)
		bike_ = new Bike(1, 5.f);
	bike_->Init(world_, bike_assets_);

	//Converting an integer to a string.
	std::string level_string;
	std::stringstream out;
	out << level_;
	level_string = out.str();

	//Loading track.
	std::string str_file_path = "track_" + level_string + ".scn";
	const char* track_file_path = str_file_path.c_str();
	track_asset_instances_.push_back(LoadSceneAsset(*platform, track_file_path));
	track_assets_.push_back(GetMeshFromSceneAssets(track_asset_instances_[track_asset_instances_.size() - 1]));
	
	//Loading .obj file as a string.
	std::string obj_file_path = "track_" + level_string + ".obj";
	obj_file_ = loadFile(obj_file_path.c_str());

	//Poplulating vector for chain shape with vertices in .obj file.
	ExtractVertices(obj_file_);

	//Initializing track.
	track_ = new Track();
	track_->Init(world_, track_assets_[0], b2Vec2(0, 0), &chain_shape_vertices_[0], (int)chain_shape_vertices_.size());

	//Loading bar.
	const char* bar_filename = "bar.scn";
	bar_asset_instances_.push_back(LoadSceneAsset(*platform, bar_filename));
	bar_assets_.push_back(GetMeshFromSceneAssets(bar_asset_instances_[bar_asset_instances_.size() - 1]));

	//Initializing bar.
	bar_ = new Bar();
	if(level_ == 1)
		bar_->Init(world_, bar_assets_[0], b2Vec2(530, 34), 1);
	else if (level_ == 2)
		bar_->Init(world_, bar_assets_[0], b2Vec2(284, 56), 1);

	SetupLights();

#if VIT_MODE
	//Turn on engine sound.
	ToggleAudioSample(0, true);
#endif
}

void GameState::HandleInput(float frame_time)
{	
	const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);

	//If TRIANGLE is pressed, reset level.
	if (controller->buttons_down() & gef_SONY_CTRL_TRIANGLE || KeyDown(gef::Keyboard::KeyCode::KC_F))
	{
		state_handler_->SetState(State::STATE_TYPE::GAME);
	}

	//If the bike has crashed, lose control of bike.
	if (!bike_->GetCrashed())
	{
		//If CROSS is pressed, accelerate.
		if (controller->buttons_down() & gef_SONY_CTRL_CROSS || KeyDown(gef::Keyboard::KeyCode::KC_W))
		{
			//If bike has been initialised.
			if (bike_)
			{
				bike_->SetAccelerating(true);
				bike_->GetBackWheelJoint()->EnableMotor(true);
				//If the bike is not going maximum speed, accelerate.
				if (bike_->GetBackWheelJoint()->GetMotorSpeed() < bike_->GetMaxSpeed())
				{
					bike_->GetBackWheelJoint()->SetMotorSpeed(bike_->GetBackWheelJoint()->GetMotorSpeed() + frame_time * bike_->GetAcceleration());
				}
			}
		}
		//If SQUARE is pressed, apply brakes.
		else if (controller->buttons_down() & gef_SONY_CTRL_SQUARE || KeyDown(gef::Keyboard::KeyCode::KC_S))
		{
			bike_->SetAccelerating(false);
			bike_->GetBackWheelJoint()->EnableMotor(true);
			bike_->GetFrontWheelJoint()->EnableMotor(true);
			bike_->GetBackWheelJoint()->SetMotorSpeed(0);
			bike_->GetFrontWheelJoint()->SetMotorSpeed(0);
		}
		//If neither is being pressed, roll.
		else {
			bike_->SetAccelerating(false);
			bike_->GetBackWheelJoint()->EnableMotor(false);
			bike_->GetFrontWheelJoint()->EnableMotor(false);
			bike_->GetBackWheelJoint()->SetMotorSpeed(bike_->GetBackWheelJoint()->GetJointSpeed());
			bike_->GetFrontWheelJoint()->SetMotorSpeed(0);
		}

		//If LEFT is pressed, lean backwards.
		if (controller->buttons_down() & gef_SONY_CTRL_LEFT || KeyDown(gef::Keyboard::KeyCode::KC_A))
		{
			//No longer leaning completely forwards.
			bike_->SetAnkleMaxForward(false);
			bike_->SetKneeMaxForward(false);
			bike_->SetHipMaxForward(false);

			//Applying an angular impulse to the bike to help with the physics and to allow rotations in the air.
			bike_->GetFrame()->GetBody()->ApplyAngularImpulse(25, true);

			//Setting motor speeds in joints to lean player backwards.
			if (bike_->GetAnkleJoint() != nullptr)
				bike_->GetAnkleJoint()->SetMotorSpeed(10);
			bike_->GetKneeJoint()->SetMotorSpeed(10);
			bike_->GetHipJoint()->SetMotorSpeed(-10);

		}
		//If RIGHT is pressed, lean forwards.
		else if (controller->buttons_down() & gef_SONY_CTRL_RIGHT || KeyDown(gef::Keyboard::KeyCode::KC_D))
		{
			//No longer leaning completely backwards.
			bike_->SetAnkleMaxBack(false);
			bike_->SetKneeMaxBack(false);
			bike_->SetHipMaxBack(false);

			//Applying an angular impulse to the bike to help with the physics and to allow rotations in the air.
			bike_->GetFrame()->GetBody()->ApplyAngularImpulse(-20, true);

			//Setting motor speeds in joints to lean player forwards.
			if (bike_->GetAnkleJoint() != nullptr)
				bike_->GetAnkleJoint()->SetMotorSpeed(-10);
			bike_->GetKneeJoint()->SetMotorSpeed(-10);
			bike_->GetHipJoint()->SetMotorSpeed(10);
		}
	}
}

void GameState::Update(float frame_time)
{
	//Setting camera angle.
	angle_ = 2.6f;
	zoom_ = 30;

	HandleInput(frame_time);

	//Update the world and handle collisions.
	UpdateSimulation(frame_time);

	//Setting the force and speed of the front suspension joint.
	//Equation for prismatic spring effect found at http://www.box2d.org/forum/viewtopic.php?t=8915.
	bike_->GetFrontSuspensionJoint()->SetMaxMotorForce(bike_->GetFrontSuspensionForce() + (40 * bike_->GetFrontSuspensionForce() * pow(bike_->GetFrontSuspensionJoint()->GetJointTranslation(), 2)));
	bike_->GetFrontSuspensionJoint()->SetMotorSpeed(-20 * bike_->GetFrontSuspensionJoint()->GetJointTranslation());


	//If the bike has not crashed.
	if (!bike_->GetCrashed())
	{
		//If leg joints are at their limits, set their motor speeds to zero.
		if (bike_->GetAnkleJoint() != nullptr)
			if ((bike_->GetAnkleMaxForward() || bike_->GetAnkleMaxBack()))
				bike_->GetAnkleJoint()->SetMotorSpeed(0);
		if ((bike_->GetKneeMaxForward() || bike_->GetKneeMaxBack()))
			bike_->GetKneeJoint()->SetMotorSpeed(0);
		if ((bike_->GetHipMaxForward() || bike_->GetHipMaxBack()))
			bike_->GetHipJoint()->SetMotorSpeed(0);

		//If leg joints get to their limits, update booleans in bike class.
		if (bike_->GetAnkleJoint() != nullptr)
		{
			if (bike_->GetAnkleJoint()->GetJointAngle() < bike_->GetAnkleJoint()->GetLowerLimit())
				bike_->SetAnkleMaxForward(true);
			if (bike_->GetAnkleJoint()->GetJointAngle() > bike_->GetAnkleJoint()->GetUpperLimit())
				bike_->SetAnkleMaxBack(true);
		}
		if (bike_->GetKneeJoint()->GetJointAngle() < bike_->GetKneeJoint()->GetLowerLimit())
			bike_->SetKneeMaxForward(true);
		if (bike_->GetKneeJoint()->GetJointAngle() > bike_->GetKneeJoint()->GetUpperLimit())
			bike_->SetKneeMaxBack(true);
		if (bike_->GetHipJoint()->GetJointAngle() < bike_->GetHipJoint()->GetLowerLimit())
			bike_->SetHipMaxForward(true);
		if (bike_->GetHipJoint()->GetJointAngle() > bike_->GetHipJoint()->GetUpperLimit())
			bike_->SetHipMaxBack(true);

		//Adjusting the pitch of the engine sound depending on the speed of the back motor.
		AdjustPitch();
	}
	//If the bike has crashed.
	else
	{
		if ((crash_timer_ += frame_time) > crash_time_)
			if(bike_->GetFrame()->GetBody()->GetPosition().x > bar_->GetBody()->GetPosition().x)
				state_handler_->SetState(State::STATE_TYPE::WIN);
			else
				state_handler_->SetState(State::STATE_TYPE::CRASHED);
	}
}

void GameState::Render(gef::Font* font, float fps)
{
	//Setting up the camera.
	float fov = gef::DegToRad(45.0f);
	float aspect_ratio = (float)platform_->width() / (float)platform_->height();
	gef::Matrix44 projection_matrix;
	projection_matrix = platform_->PerspectiveProjectionFov(fov, aspect_ratio, 0.1f, 400.0f);
	renderer_3d_->set_projection_matrix(projection_matrix);
	gef::Vector4 camera_eye;
	gef::Vector4 camera_lookat;

	if (!bike_->GetCrashed())
	{
		camera_eye.set_value(bike_->GetFrame()->GetBody()->GetWorldCenter().x + cos(angle_) * zoom_, bike_->GetFrame()->GetBody()->GetWorldCenter().y, sin(angle_) * zoom_);
		camera_lookat.set_value(bike_->GetFrame()->GetBody()->GetWorldCenter().x, bike_->GetFrame()->GetBody()->GetWorldCenter().y, 0.0f);
	}
	else
	{
		camera_eye.set_value(bike_->GetRiderBody()->GetBody()->GetWorldCenter().x + cos(angle_) * zoom_, bike_->GetRiderBody()->GetBody()->GetWorldCenter().y, sin(angle_) * zoom_);
		camera_lookat.set_value(bike_->GetRiderBody()->GetBody()->GetWorldCenter().x, bike_->GetRiderBody()->GetBody()->GetWorldCenter().y, 0.0f);
	}

	gef::Vector4 camera_up(0.0f, 1.0f, 0.0f);
	gef::Matrix44 view_matrix;
	view_matrix.LookAt(camera_eye, camera_lookat, camera_up);
	renderer_3d_->set_view_matrix(view_matrix);

	//Drawing 3d geometry.
	renderer_3d_->Begin();
	renderer_3d_->set_override_material(NULL);
	renderer_3d_->DrawMesh(*bike_->GetBackWheel());
	renderer_3d_->DrawMesh(*bike_->GetBackSuspension());
	renderer_3d_->DrawMesh(*bike_->GetFrame());
	renderer_3d_->DrawMesh(*bike_->GetFrontSuspension());
	renderer_3d_->DrawMesh(*bike_->GetFrontWheel());
	renderer_3d_->DrawMesh(*bike_->GetRiderLowerLeg());
	renderer_3d_->DrawMesh(*bike_->GetRiderUpperLeg());
	renderer_3d_->DrawMesh(*bike_->GetRiderBody());
	renderer_3d_->DrawMesh(*bike_->GetRiderUpperArm());
	renderer_3d_->DrawMesh(*bike_->GetRiderLowerArm());
	renderer_3d_->DrawMesh(*bike_->GetRiderHead());
	renderer_3d_->DrawMesh(*track_);
	renderer_3d_->DrawMesh(*bar_);
	renderer_3d_->End();

	//Start drawing sprites, but don't clear the frame buffer.
	sprite_renderer_->Begin(false);
	DrawHUD(sprite_renderer_, font, fps);
	sprite_renderer_->End();
}

void GameState::Release()
{
	//Destroying the physics world also destroys all the objects within it.
	delete world_;
	world_ = NULL;

	delete primitive_builder_;
	primitive_builder_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

	delete bike_;
	bike_ = NULL;

	//Disabling engine sound.
	ToggleAudioSample(0, false);
}

void GameState::SetupLights()
{
	//Grab the data for the default shader used for rendering 3D geometry.
	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();

	default_shader_data.CleanUp();

	//Set the ambient light.
	default_shader_data.set_ambient_light_colour(gef::Colour(255.0f, 255.0f, 255.0f, 1.0f));// gef::Colour(0.25f, 0.25f, 0.25f, 1.0f));

	//The position of the light is set far away so it acts light a directional light.
	gef::PointLight point_light_1;
	point_light_1.set_colour(gef::Colour(255.0f, 255.0f, 255.0f, 1.0f));
	point_light_1.set_position(gef::Vector4(300, 100, 70));
	default_shader_data.AddPointLight(point_light_1);
}

void GameState::UpdateSimulation(float frame_time)
{
	//Update physics world.
	float32 timeStep = 1.0f / 60;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world_->Step(timeStep, velocityIterations, positionIterations);

	//Update all of bike's GameObjects.
	bike_->GetBackWheel()->UpdateFromSimulation();
	bike_->GetBackSuspension()->UpdateFromSimulation();
	bike_->GetFrame()->UpdateFromSimulation();
	bike_->GetFrontSuspension()->UpdateFromSimulation();
	bike_->GetFrontWheel()->UpdateFromSimulation();
	bike_->GetRiderLowerLeg()->UpdateFromSimulation();
	bike_->GetRiderUpperLeg()->UpdateFromSimulation();
	bike_->GetRiderBody()->UpdateFromSimulation();
	bike_->GetRiderUpperArm()->UpdateFromSimulation();
	bike_->GetRiderLowerArm()->UpdateFromSimulation();
	bike_->GetRiderHead()->UpdateFromSimulation();

	//Don't have to update the track as it is static.

	//Collision detection.

	//Get the head of the contact list.
	b2Contact* contact = world_->GetContactList();

	//Get contact count.
	int contact_count = world_->GetContactCount();

	//Collision response.
	for (int contact_num = 0; contact_num<contact_count; ++contact_num)
	{
		if (contact->IsTouching())
		{
			//Get the colliding bodies.
			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();

			GameObject* gameObjectA = NULL;
			GameObject* gameObjectB = NULL;

			gameObjectB = (GameObject*)bodyB->GetUserData();
			gameObjectA = (GameObject*)bodyA->GetUserData();

			//If the bike has not crashed.
			if (!bike_->GetCrashed())
			{
				//If any body of the rider (except the lower arm and leg) hits the track.
				if ((gameObjectA != nullptr && (gameObjectA->GetType() == RIDER_UPPER_LEG || gameObjectA->GetType() == RIDER_UPPER_ARM || gameObjectA->GetType() == RIDER_BODY || gameObjectA->GetType() == RIDER_HEAD) &&
					gameObjectB != nullptr && gameObjectB->GetType() == ENVIRONMENT) ||
					(gameObjectB != nullptr && (gameObjectB->GetType() == RIDER_UPPER_LEG || gameObjectB->GetType() == RIDER_UPPER_ARM || gameObjectB->GetType() == RIDER_BODY || gameObjectB->GetType() == RIDER_HEAD) &&
					gameObjectA != nullptr && gameObjectA->GetType() == ENVIRONMENT)) {

					bike_->FallOff();
				}
			}
		}
		//Get the next contact point.
		contact = contact->GetNext();
	}
}

gef::Scene* GameState::LoadSceneAsset(gef::Platform& platform, const char* filename)
{
	gef::Scene* scene = new gef::Scene();														//Are these gef::Scene pointers being deleted as they are in a vector?					
	//If scene file loads successfully.
	if (scene->ReadSceneFromFile(platform, filename))
	{
		//Create material and mesh resources from the scene data.
		scene->CreateMaterials(platform);
		scene->CreateMeshes(platform);
	}
	else
		scene = nullptr;
	return scene;
}

gef::Mesh* GameState::GetMeshFromSceneAssets(gef::Scene* scene)
{
	gef::Mesh* mesh = nullptr;																	//Do i need to delete this gef::Mesh pointer. If so, how do I do that as I need to return it first?
	//If the scene data contains at least one mesh.
	if (scene && scene->meshes.size() > 0)
		mesh = scene->meshes.front();
	//Return the first mesh.
	return mesh;
}

std::string GameState::loadFile(const char* file_name)
{
	//This code is adapted from gef::Font::Load().
	std::string config_filename(file_name);
	void* file_data = nullptr;
	Int32 file_size = 0;
	gef::File* file = gef::File::Create();

	bool success = true;
	success = file->Open(config_filename.c_str());
	if (success)
	{
		success = file->GetSize(file_size);
		if (success)
		{
			file_data = malloc(file_size);
			success = file_data != nullptr;
			if (success)
			{
				Int32 bytes_read;
				success = file->Read(file_data, file_size, bytes_read);
				if (success)
					success = bytes_read == file_size;
			}
		}
		file->Close();
		delete file;
		file = nullptr;
	}
	std::string str((char*)file_data);
	return str;
}

void GameState::ExtractVertices(std::string s)
{
	//Reading every character in the string.
	for (int i = 0; i < (int)s.length(); i++)
	{
		//If the character is not the last one in the string.
		if (i < (int)s.length() - 1)
		{
			//If the string "v " is detected, the next characters will have information about vertices.
			if (s[i] == 'v' && s[i + 1] == ' ')
			{
				//Jump to the begining of the number.
				i += 2;
				//Save the starting index.
				int start = i;
				//Iterate forwards until a space is found.
				while (s[i++] != ' ') {}
				//Create a string from the character at the saved index to the character at the current index.
				std::string xs(s, start, i - start);
				//Convert that string into a float and storing it as the x value of the vertex.
				float x = (float)std::atof(xs.c_str());
				//Repeat the process for the y value.
				start = i;
				while (s[i++] != ' ') {}
				std::string ys(s, start, i - start);
				float y = (float)std::atof(ys.c_str());
				//Creating a b2Vec2 using the x and y values found.
				b2Vec2 vertex(x, y);
				//Looping through the chain shape vertices array and only adding the b2Vec2 if it is not a duplicate.
				bool duplicate = false;
				for (auto v : chain_shape_vertices_)
					//Due to floating point inaccuracies, vertices that are very close to (not exactly the same) existing vertices are counted as duplicates.
					if (v.x > vertex.x - 0.01 && v.x < vertex.x + 0.01 && v.y > vertex.y - 0.01 && v.y < vertex.y + 0.01)
					{
						duplicate = true;
						break;
					}
				//If vertex is not a duplicate, add it to the vector.
				if(!duplicate)
					chain_shape_vertices_.push_back(vertex);
			}
		}
	}
	//verts_list.sort(verts_list.begin(), verts_list.end(), [this](const b2Vec2 &a, const b2Vec2 &b) { return a.x < b.x; });

	//I had to use my own sort algorithm here as std::vector.sort() wasn't working on vita. Before c++ 11?
	BubbleSort(chain_shape_vertices_, (int)chain_shape_vertices_.size());
}

void GameState::BubbleSort(std::vector<b2Vec2>& vertices, int size)
{
	//Algorithm adapted from https://www.geeksforgeeks.org/bubble-sort/.
	int i, j;
	bool swapped;
	for (i = 0; i < size - 1; i++)
	{
		swapped = false;
		for (j = 0; j < size - i - 1; j++)
		{
			if (vertices[j].x > vertices[j + 1].x)
			{
				Swap(&vertices[j], &vertices[j + 1]);
				swapped = true;
			}
		}
		if (swapped == false)
			break;
	}
}

void GameState::Swap(b2Vec2* a, b2Vec2* b)
{
	//Swapping the b2Vec2s' locations.
	b2Vec2 temp = *a;
	*a = *b;
	*b = temp;
}

void GameState::AdjustPitch()
{
#if VITA_MODE
	//Adjusting the pitch in relation to the speed of the back wheel's motor.
	//Using Map() function added to gef namespace in math math_utils.h.
	if (bike_->GetAccelerating())
		audio_manager_->SetSamplePitch(0, gef::Map(bike_->GetBackWheelJoint()->GetMotorSpeed(), 0.0f, bike_->GetMaxSpeed(), 1.6f, 2.0f));
	else
		audio_manager_->SetSamplePitch(0, 1.6f);
#endif
}

void GameState::ToggleAudioSample(int voice_index, bool toggle)
{
#if VITA_MODE
	//Turns a sound on and off.
	if (!toggle)
		audio_manager_->StopPlayingSampleVoice(voice_index);
	else
	{
		audio_manager_->PlaySample(sfx_id_, true);
		audio_manager_->SetSamplePitch(0, 1.5);
	}
#endif
}


