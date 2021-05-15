#pragma once

#include "state.h"

#include "maths/vector4.h"
#include <graphics/sprite.h>


namespace gef
{
	class font;
	class Texture;
}

class WinState : public State
{
public:
	WinState();
	~WinState();

	void Init(gef::Platform* platform, gef::InputManager* input_manager, gef::SpriteRenderer* sprite_renderer, gef::AudioManager* audio_manager, StateHandler* state_handler, int level, int sfx_id);
	void HandleInput(float frame_time);
	void Update(float frame_time);
	void Render(gef::Font* font, float fps);
	void Release();

private:
	//Texture.
	gef::Texture* texture_;

	//Sprites.
	gef::Sprite sprite_;

	int level_;
};

