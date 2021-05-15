#include "state.h"

#include <input/input_manager.h>
#include <graphics/font.h>
#include <maths/vector4.h>

State::State()
{
}

State::~State()
{

}

bool State::KeyDown(gef::Keyboard::KeyCode key)
{
	return input_manager_->keyboard() && input_manager_->keyboard()->IsKeyDown(key);
}

void State::DrawHUD(gef::SpriteRenderer* sprite_renderer, gef::Font* font, float fps)
{
	if (font)
	{
		//Display frame rate
		font->RenderText(sprite_renderer, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps);
	}
}
