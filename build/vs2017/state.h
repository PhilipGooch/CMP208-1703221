#pragma once

#include "input/keyboard.h"

//int32 includes
//#include <system/application.h>
//#include <maths/vector2.h>
//#include "primitive_builder.h"
//#include <graphics/mesh_instance.h>
//#include <audio/audio_manager.h>
//#include <input/input_manager.h>
//#include <box2d/Box2D.h>
//#include "game_object.h"

#define VITA_MODE 1

namespace gef
{
	class Platform;
	class InputManager;
	class SpriteRenderer;
	class AudioManager;
	class Font;
}

class StateHandler;

class State
{
public:
	State();

	//Deconstuctor must be a pure virtual method to be sure to be calling the correct deconstructor.
	virtual ~State() = 0;

	//Types the State can be.
	enum STATE_TYPE
	{
		MENU,
		GAME,
		OPTIONS,
		CRASHED,
		WIN
	};

	//"level" is only used in GameState.
	virtual void Init(gef::Platform* platform, gef::InputManager* input_manager, gef::SpriteRenderer* sprite_renderer, gef::AudioManager* audio_manager, StateHandler* state_handler, int level, int sfx_id) {}
	virtual void HandleInput(float frame_time) = 0;
	virtual void Update(float frame_time) = 0;
	virtual void Render(gef::Font* font, float fps) = 0;
	virtual void Release() = 0;

	//Returns if the key passed in is down or not.
	bool KeyDown(gef::Keyboard::KeyCode key);

	//Gets the type of state.
	inline STATE_TYPE GetType() { return type_; }

protected:
	gef::Platform* platform_;
	gef::InputManager* input_manager_;
	gef::SpriteRenderer* sprite_renderer_;
	gef::AudioManager* audio_manager_;
	int sfx_id_;

	STATE_TYPE type_;

	//Render fps counter.
	void DrawHUD(gef::SpriteRenderer* sprite_renderer, gef::Font* font, float fps);

	StateHandler* state_handler_;
};

