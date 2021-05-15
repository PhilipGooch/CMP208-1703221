#include "scene_app.h"

#include "menu_state.h"
#include "game_state.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <input/input_manager.h>
#include <system/application.h>
#include <input/sony_controller_input_manager.h>
#include <audio/audio_manager.h>

SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	input_manager_(NULL),
	font_(NULL)
{
}

void SceneApp::Init()
{
	//Initialising sprite renderer.
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

	//Initialising font.
	InitFont();

	//Initialising input manager.
	input_manager_ = gef::InputManager::Create(platform_);

	//Initialising input manager.
	audio_manager_ = gef::AudioManager::Create();

	//Initialising state handler.
	state_handler_.Init(&platform_, input_manager_, sprite_renderer_, audio_manager_);
}

void SceneApp::CleanUp()			
{
	delete input_manager_;
	input_manager_ = NULL;

	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	state_handler_.Release();

	delete input_manager_;
	input_manager_ = NULL;

	//Safely destroying font.
	CleanUpFont();
}

bool SceneApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;

	input_manager_->Update();

	state_handler_.GetState()->Update(frame_time);

#if !VITA_MODE
	const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);
	//If ESCAPE is pressed, exit the application.
	if (state_handler_.GetState()->KeyDown(gef::Keyboard::KeyCode::KC_ESCAPE))
		return false;
#endif

	return true;
}

void SceneApp::Render()
{
	state_handler_.GetState()->Render(font_, fps_);
}

void SceneApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void SceneApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}








