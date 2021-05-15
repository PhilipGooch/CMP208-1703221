#pragma once

#include "state.h"

#include <vector>
#include <box2d/Box2D.h>

//Forward declarations.
namespace gef
{
	class Scene;
	class Renderer3D;
	class Mesh;
}

class PrimitiveBuilder;
class Bike;
class Track;
class Bar;

class GameState : public State
{
public:
	GameState();

	void Init(gef::Platform* platform, gef::InputManager* input_manager, gef::SpriteRenderer* sprite_renderer, gef::AudioManager* audio_manager, StateHandler* state_handler, int level, int sfx_id);

	//Setters.
	void SetLevel(int level) { level_ = level; }

	//Geters.
	int GetLevel() { return level_; }
	
	void HandleInput(float frame_time);
	void Update(float frame_time);
	void Render(gef::Font* font, float fps);
	void Release();

private:
	b2World* world_;
	gef::Renderer3D* renderer_3d_;
	PrimitiveBuilder* primitive_builder_;

	//Level.
	int level_;

	//Amount of time since crash.
	float crash_timer_;

	//Amount of time until switching states.
	float crash_time_;

	//Variables for camera angle.
	float angle_;
	float zoom_;

	//Creating bike pointer.
	Bike* bike_;

	//Vectors of objects for loading bike models.
	std::vector<std::string> bike_filenames_;
	std::vector<gef::Scene*> bike_asset_instances_;
	std::vector<gef::Mesh*> bike_assets_;

	//Creating track pointer.
	Track* track_;

	//Vectors of objects for loading track models.
	std::vector<gef::Scene*> track_asset_instances_;
	std::vector<gef::Mesh*> track_assets_;

	//Creating bar pointer.
	Bar* bar_;

	//Vectors of objects for loading track models.
	std::vector<gef::Scene*> bar_asset_instances_;
	std::vector<gef::Mesh*> bar_assets_;

	//String holding the object file.
	std::string obj_file_;

	//Loads a file into a string.
	std::string loadFile(const char* file_name);

	//Vector of vertices used for the tracks chain shape.
	std::vector<b2Vec2> chain_shape_vertices_;

	//Populates chain_shape_vertices_ with the vertices declared in the obj file.
	void ExtractVertices(std::string s);

	//Loads in an asset with a given filename.
	gef::Scene* LoadSceneAsset(gef::Platform& platform, const char* filename);

	//Gets the first mesh in the meshs vector.
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

	//Sorting algorithm that sorts the vertices by their x value from lowest to highest.
	void BubbleSort(std::vector<b2Vec2>& vertices, int number);

	//Swaps the position of two b2Vec2s in a vector.
	void Swap(b2Vec2* a, b2Vec2* b);

	//Adjusts the pitch of the engine sample depending on the speed of the back motor.
	void AdjustPitch();

	//Toggles a sample sound on and off.
	void ToggleAudioSample(int voice_index, bool toggle);

	//Sets up the lights.
	void SetupLights();

	//Updates the world. Collisions are handled here.
	void UpdateSimulation(float frame_time);

};

