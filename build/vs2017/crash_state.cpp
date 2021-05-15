#include "crash_state.h"

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

CrashState::CrashState()
{
}

CrashState::~CrashState()
{
}

void CrashState::Init(gef::Platform* platform, gef::InputManager* input_manager, gef::SpriteRenderer* sprite_renderer, gef::AudioManager* audio_manager, StateHandler* state_handler, int level, int sfx_id)
{
	type_ = CRASHED;

	platform_ = platform;
	input_manager_ = input_manager;
	sprite_renderer_ = sprite_renderer;
	audio_manager_ = audio_manager;
	state_handler_ = state_handler;

	//Creating sprites.
	texture_ = CreateTextureFromPNG("fall.png", *platform);

	//Setting sprite variables.
	sprite_.set_texture(texture_);
	sprite_.set_position(gef::Vector4(platform_->width() * 0.5f, platform_->height() * 0.5f, -0.99f));
	sprite_.set_width(960);
	sprite_.set_height(544);
}

void CrashState::HandleInput(float frame_time)
{
	const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);
	if (controller->buttons_down() & gef_SONY_CTRL_CROSS || KeyDown(gef::Keyboard::KeyCode::KC_F))
		state_handler_->SetState(State::STATE_TYPE::GAME);
	if (controller->buttons_down() & gef_SONY_CTRL_SQUARE || KeyDown(gef::Keyboard::KeyCode::KC_C))
		state_handler_->SetState(State::STATE_TYPE::MENU);
}

void CrashState::Update(float frame_time)
{
	HandleInput(frame_time);
}

void CrashState::Render(gef::Font* font, float fps)
{
	sprite_renderer_->Begin();

	//Render sprites.
	sprite_renderer_->DrawSprite(sprite_);

	sprite_renderer_->End();
}

void CrashState::Release()
{
	//Delete textures.
	delete texture_;
	texture_ = nullptr;
}
