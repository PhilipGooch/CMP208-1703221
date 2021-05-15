#include "options_state.h"

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
#include <sstream>

#include <system/debug_log.h>

OptionsState::OptionsState()
{
}

OptionsState::~OptionsState()
{
}

void OptionsState::Init(gef::Platform* platform, gef::InputManager* input_manager, gef::SpriteRenderer* sprite_renderer, gef::AudioManager* audio_manager, StateHandler* state_handler, int level, int sfx_id)
{
	type_ = OPTIONS;

	platform_ = platform;
	input_manager_ = input_manager;
	sprite_renderer_ = sprite_renderer;
	audio_manager_ = audio_manager;
	state_handler_ = state_handler;

	//Creating sprites.
	texture_ = CreateTextureFromPNG("options.png", *platform);

	//Setting sprite variables.
	sprite_.set_texture(texture_);
	sprite_.set_position(gef::Vector4(platform_->width() * 0.5f, platform_->height() * 0.5f, -0.99f));
	sprite_.set_width(960);
	sprite_.set_height(544);

	//Initializing all text sizes.
	for (int i = 0; i < levels_; i++)
		text_sizes_[i] = text_min_size_;

	//Initializing grow booleans.
	for (int i = 0; i < levels_; i++)
		grow[i] = false;
}

void OptionsState::HandleInput(float frame_time)
{
	const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);
	if (controller->buttons_down() & gef_SONY_CTRL_SQUARE || KeyDown(gef::Keyboard::KeyCode::KC_C))
	{
		state_handler_->SetState(State::STATE_TYPE::MENU);
	}
	if (controller->buttons_down() & gef_SONY_CTRL_LEFT || KeyDown(gef::Keyboard::KeyCode::KC_A))
	{
		if (selection > 0)
			selection--;
		state_handler_->SetLevel(selection + 1);
	}
	if (controller->buttons_down() & gef_SONY_CTRL_RIGHT || KeyDown(gef::Keyboard::KeyCode::KC_D))
	{
		if (selection < levels_ - 1)
			selection++;
		state_handler_->SetLevel(selection + 1);
	}
}

void OptionsState::Update(float frame_time)
{
	HandleInput(frame_time);

	//Updating font sizes.
	for (int i = 0; i < levels_; i++)
	{
		if (selection == i)
		{
			//Reseting sizes of all other text.
			for(int j = 0; j < levels_; j++)
				if(j != i)
					text_sizes_[j] = text_min_size_;
			//Setting whether it is growing or shrinking.
			if (text_sizes_[i] >= text_max_size_)
				grow[i] = false;
			else if (text_sizes_[i] <= text_min_size_)
				grow[i] = true;
			if (grow[i])
				text_sizes_[i] += frame_time;
			else
				text_sizes_[i] -= frame_time;
		}
	}
	gef::DebugOut("%i\n", grow[0]);
}

void OptionsState::Render(gef::Font* font, float fps)
{
	sprite_renderer_->Begin();

	//Render sprites.
	sprite_renderer_->DrawSprite(sprite_);

	//Render "PLAY" text.
	for (int i = 0; i < levels_; i++)
	{
		std::string number_string;
		std::stringstream out;
		out << i + 1;
		number_string = out.str();
		font->RenderText(
			sprite_renderer_,
			gef::Vector4(platform_->width()*0.42f + 100 * i, platform_->height()*0.5f + 45.0f, -0.99f),
			text_sizes_[i],
			0xff000000,
			gef::TJ_CENTRE,
			number_string.c_str());
	}

	sprite_renderer_->End();
}

void OptionsState::Release()
{
	//Delete textures.
	delete texture_;
	texture_ = nullptr;
}
