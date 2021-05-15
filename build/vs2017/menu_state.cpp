#include "menu_state.h"

#include "primitive_builder.h"
#include <input/input_manager.h>
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include "load_texture.h"
#include <graphics/font.h>
#include <maths/math_utils.h>
#include <input/sony_controller_input_manager.h>
#include <math.h>
#include <audio/audio_manager.h>
#include "state_handler.h"

MenuState::MenuState()
{
}

MenuState::~MenuState()
{
}

void MenuState::Init(gef::Platform* platform, gef::InputManager* input_manager, gef::SpriteRenderer* sprite_renderer, gef::AudioManager* audio_manager, StateHandler* state_handler, int level, int sfx_id)
{
	type_ = MENU;

	platform_ = platform;
	input_manager_ = input_manager;
	sprite_renderer_ = sprite_renderer;
	audio_manager_ = audio_manager;
	state_handler_ = state_handler;

	selection_ = 0;

	//Creating textures.
	button_icon_texture_ = CreateTextureFromPNG("playstation-cross-dark-icon.png", *platform);
	rider_texture_ = CreateTextureFromPNG("rider.png", *platform);
	sky_texture_ = CreateTextureFromPNG("sky.png", *platform);
	controls_texture_ = CreateTextureFromPNG("controls.png", *platform);
	title_texture_ = CreateTextureFromPNG("trials.png", *platform);

	//Setting rider_sprite_ positions.
	rider_position_.set_x(platform_->width() * 0.3f);
	rider_position_.set_y(platform_->height() * 0.3f);
	rider_position_.set_z(-0.99f);

	//Setting sprite variables.
	sky_sprite_.set_texture(sky_texture_);
	sky_sprite_.set_position(gef::Vector4(platform_->width()*0.5f, platform_->height()*0.5f, -0.99f));
	sky_sprite_.set_width(960);
	sky_sprite_.set_height(544);

	rider_sprite_.set_texture(rider_texture_);
	rider_sprite_.set_position(rider_position_);
	rider_sprite_.set_width(248);
	rider_sprite_.set_height(272);

	controls_sprite_.set_texture(controls_texture_);
	controls_sprite_.set_position(gef::Vector4(platform_->width()*0.35f + 0.f, platform_->height()*0.75f + 10.f, -0.99f));
	controls_sprite_.set_width((957 / 2) * 1.2f);
	controls_sprite_.set_height((364 / 2) * 1.2f);

	title_sprite_.set_texture(title_texture_);
	title_sprite_.set_position(gef::Vector4(platform_->width()*0.75f - 50.f, platform_->height()*0.25f + 50.f, -0.99f));
	title_sprite_.set_width(464);
	title_sprite_.set_height(180);

	//Creating a target for the bike sprite.
	GenerateNewTarget();

#if 0
	//Load music.
	audio_manager_->LoadMusic("music.wav", *platform);
	//Setup music volume.
	gef::VolumeInfo music_volume_info;
	music_volume_info.volume = 0.3;
	audio_manager_->SetMusicVolumeInfo(music_volume_info);

	//Play music.
	audio_manager_->PlayMusic();
#endif
}

void MenuState::HandleInput(float frame_time)
{
	const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);
	//If UP is pressed, move selection up.
	if (controller->buttons_down() & gef_SONY_CTRL_UP || KeyDown(gef::Keyboard::KeyCode::KC_W))
		if (selection_ == 1)
			selection_ = 0;
	//If DOWN is pressed, move selection down.
	if (controller->buttons_down() & gef_SONY_CTRL_DOWN || KeyDown(gef::Keyboard::KeyCode::KC_S))
		if (selection_ == 0)
			selection_ = 1;
	if (controller->buttons_down() & gef_SONY_CTRL_CROSS || KeyDown(gef::Keyboard::KeyCode::KC_F))
		if(selection_ == 0)
			state_handler_->SetState(State::STATE_TYPE::GAME);
		else if (selection_ == 1)
			state_handler_->SetState(State::STATE_TYPE::OPTIONS);
}

void MenuState::GenerateNewTarget()
{
	//Setting the rider's target position to a random point in a radius around a fixed point.
	int random = rand() % 10 - 5;
	rider_target_position_.set_x(platform_->width() * 0.3f + cosf(rider_sprite_angle_ + gef::DegToRad((float)random)) * (rand() % 25 + 10));
	rider_target_position_.set_y(platform_->height() * 0.3f + sinf(rider_sprite_angle_ + gef::DegToRad((float)random)) * (rand() % 25 + 10));
	rider_target_position_.set_z(-0.99f);
	rider_sprite_angle_ = rider_sprite_angle_ + gef::DegToRad((float)random);
}

void MenuState::Update(float frame_time)
{
	HandleInput(frame_time);

	//Using Lerp() function to smoothly move to new position.
	float new_x = gef::Lerp(rider_position_.x(), rider_target_position_.x(), .01f);
	float new_y = gef::Lerp(rider_position_.y(), rider_target_position_.y(), .01f);

	GenerateNewTarget();

	rider_position_.set_x(new_x);
	rider_position_.set_y(new_y);

	rider_sprite_.set_position(rider_position_);

	//Animating the menu text.
	//If PLAY is selected.
	if (selection_ == 0)
	{
		options_size_ = menu_min_size_;
		if (play_size_ > menu_max_size_)
			grow_ = false;
		if (play_size_ < menu_min_size_)
			grow_ = true;
		if (grow_)
			play_size_ += frame_time;
		else
			play_size_ -= frame_time;
	}
	//If OPTIONS is selected.
	else if (selection_ == 1)
	{
		play_size_ = menu_min_size_;
		if (options_size_ > menu_max_size_)
			grow_ = false;
		if (options_size_ < menu_min_size_)
			grow_ = true;
		if (grow_)
			options_size_ += frame_time;
		else
			options_size_ -= frame_time;
	}
}

void MenuState::Render(gef::Font* font, float fps)
{
	sprite_renderer_->Begin();

	//Render sprites.
	sprite_renderer_->DrawSprite(sky_sprite_);

	sprite_renderer_->DrawSprite(rider_sprite_);

	sprite_renderer_->DrawSprite(controls_sprite_);

	sprite_renderer_->DrawSprite(title_sprite_);

	//Render "PLAY" text.
	font->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_->width()*0.75f, platform_->height()*0.5f + 25.0f, -0.99f),
		play_size_,
		0xff000000,
		gef::TJ_CENTRE,
		"PLAY");

	//Render "OPTIONS" text.
	font->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_->width()*0.75f + 50.f, platform_->height()*0.5f + 125.0f, -0.99f),
		options_size_,
		0xff000000,
		gef::TJ_CENTRE,
		"OPTIONS");
	
	sprite_renderer_->End();
}

void MenuState::Release()
{
	//Delete textures.
	delete button_icon_texture_;
	button_icon_texture_ = nullptr;

	delete rider_texture_;
	rider_texture_ = nullptr;

	delete sky_texture_;
	sky_texture_ = nullptr;

#if 0
	//Safely destroying sound files in audio manager.
	//audio_manager_->UnloadMusic();
	//audio_manager_->UnloadSample(sfx_id_);
#endif
}
