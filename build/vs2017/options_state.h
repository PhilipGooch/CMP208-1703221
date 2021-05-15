#pragma once

#include "state.h"

#include "maths/vector4.h"
#include <graphics/sprite.h>


namespace gef
{
	class font;
	class Texture;
}

class OptionsState : public State
{
public:
	OptionsState();
	~OptionsState();

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

	static const int levels_ = 2;

	//Maximum and minimum sizes for the menu text. Used for selection.
	float text_min_size_ = 3.0f;
	float text_max_size_ = 3.5f;

	float text_sizes_[levels_];


	//Boolean for text animation.
	bool grow[levels_];

	//What option is currently selected.
	int selection = 0;
};

