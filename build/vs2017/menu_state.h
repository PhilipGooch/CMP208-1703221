#pragma once

#include "state.h"

#include "maths/vector4.h"
#include <graphics/sprite.h>


namespace gef
{
	class font;
	class Texture;
}

class MenuState : public State
{
public:
	MenuState();
	~MenuState();

	void Init(gef::Platform* platform, gef::InputManager* input_manager, gef::SpriteRenderer* sprite_renderer, gef::AudioManager* audio_manager, StateHandler* state_handler, int level, int sfx_id);
	void HandleInput(float frame_time);
	void Update(float frame_time);
	void Render(gef::Font* font, float fps);
	void Release();

	//Getters.
	inline int GetSelection() { return selection_; }

private:
	//Textures.
	gef::Texture* button_icon_texture_;
	gef::Texture* rider_texture_;
	gef::Texture* sky_texture_;
	gef::Texture* controls_texture_;
	gef::Texture* title_texture_;

	//Sprites.
	gef::Sprite rider_sprite_;
	gef::Sprite sky_sprite_;
	gef::Sprite controls_sprite_;
	gef::Sprite title_sprite_;

	//Position vectors used for sprite animation.
	gef::Vector4 rider_position_;
	gef::Vector4 rider_target_position_;

	//Creates a target vector the bike sprite is aiming for.
	void GenerateNewTarget();

	//Variable used in GenerateNewTarget().
	float rider_sprite_angle_ = 0;
	
	//Maximum and minimum sizes for the menu text. Used for selection.
	float menu_min_size_ = 2.0f;
	float menu_max_size_ = 2.5f;

	//Initializing both menu text's sizes to minimum as they are not selected on load.
	float play_size_ = menu_min_size_;
	float options_size_ = menu_min_size_;

	//Boolean for text animation.
	bool grow_ = true;
	
	//What option is currently selected.
	int selection_ = 0;
};

