#pragma once

//Including this in the header file as I need access to State::STATE_TYPE.
#include "state.h"

namespace gef
{
	class Platform;
	class InputManager;
	class SpriteRenderer;
	class AudioManager;
}
class MenuState;
class GameState;
class CrashState;
class WinState;
class OptionsState;

class StateHandler
{
public:
	StateHandler();
	~StateHandler();

	void Init(gef::Platform* platform, gef::InputManager* input_manager, gef::SpriteRenderer* sprite_renderer, gef::AudioManager* audio_manager);

	//Setters.
	inline void SetLevel(int level) { level_ = level; }

	//Geters.
	inline int GetLevel() { return level_; }

	//Changs the state of the application.
	void SetState(State::STATE_TYPE type);

	//Reteruns the state of the application.
	inline State* GetState() { return state_; }

	void Release();

private:
	gef::Platform* platform_;
	gef::InputManager* input_manager_;
	gef::SpriteRenderer* sprite_renderer_;
	gef::AudioManager* audio_manager_;
	int sfx_id_;

	//All states.
	GameState* game_state_;
	MenuState* menu_state_;
	CrashState* crash_state_;
	WinState* win_state_;
	OptionsState* options_state_;

	//Level of game state.
	int level_;

	//Current state.
	State* state_;	
};

