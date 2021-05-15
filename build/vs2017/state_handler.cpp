#include "state_handler.h"
#include "game_state.h"
#include "menu_state.h"
#include "crash_state.h"
#include "win_state.h"
#include "options_state.h"
#include <audio/audio_manager.h>

StateHandler::StateHandler()
{
}

StateHandler::~StateHandler()
{
}

void StateHandler::Init(gef::Platform* platform, gef::InputManager* input_manager, gef::SpriteRenderer* sprite_renderer, gef::AudioManager* audio_manager)
{
	platform_ = platform;
	input_manager_ = input_manager;
	sprite_renderer_ = sprite_renderer;
	audio_manager_ = audio_manager;

#if VITA_MODE
	//Loading music and setting volume.
	audio_manager_->LoadMusic("music.wav", *platform);
	gef::VolumeInfo music_volume_info;
	music_volume_info.volume = 0.3;
	audio_manager_->SetMusicVolumeInfo(music_volume_info);

	//Loading sample sounds and setting volume.
	sfx_id_ = audio_manager_->LoadSample("engine_2.wav", *platform);
	gef::VolumeInfo sample_volume_info;
	sample_volume_info.volume = 0.5;
	sample_volume_info.pan = 0;
	audio_manager_->SetSampleVoiceVolumeInfo(sfx_id_, sample_volume_info);

	audio_manager_->PlayMusic();
#endif

	level_ = 1;

	game_state_ = new GameState();
	menu_state_ = new MenuState();
	crash_state_ = new CrashState();
	win_state_ = new WinState();
	options_state_ = new OptionsState();

	state_ = menu_state_;
	//Passing this instance of this class to the state.
	state_->Init(platform_, input_manager_, sprite_renderer_, audio_manager_, this, -1, sfx_id_);
}

void StateHandler::SetState(State::STATE_TYPE type)
{
	state_->Release();
	if (type == State::STATE_TYPE::MENU)
	{
		state_ = menu_state_;
		state_->Init(platform_, input_manager_, sprite_renderer_, audio_manager_, this, level_, sfx_id_);
	}
	else if (type == State::STATE_TYPE::GAME)
	{
		state_ = game_state_;
		state_->Init(platform_, input_manager_, sprite_renderer_, audio_manager_, this, level_, sfx_id_);
	}
	else if (type == State::STATE_TYPE::CRASHED)
	{
		state_ = crash_state_;
		state_->Init(platform_, input_manager_, sprite_renderer_, audio_manager_, this, level_, sfx_id_);
	}
	else if (type == State::STATE_TYPE::WIN)
	{
		state_ = win_state_;
		state_->Init(platform_, input_manager_, sprite_renderer_, audio_manager_, this, level_, sfx_id_);
	}
	else if (type == State::STATE_TYPE::OPTIONS)
	{
		state_ = options_state_;
		state_->Init(platform_, input_manager_, sprite_renderer_, audio_manager_, this, level_, sfx_id_);
	}
}

void StateHandler::Release()
{
	audio_manager_->UnloadMusic();
	audio_manager_->UnloadAllSamples();
}

