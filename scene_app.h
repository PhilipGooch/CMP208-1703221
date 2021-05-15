#ifndef _SCENE_APP_H
#define _SCENE_APP_H

#include <system/application.h>
#include "state_handler.h"


class State;

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class InputManager;
	class AudioManager;
}

class SceneApp : public gef::Application
{
public:
	SceneApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();

private:
	void InitFont();
	void CleanUpFont();
    
	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
	gef::InputManager* input_manager_;
	gef::AudioManager* audio_manager_;

	//Frames per second.
	float fps_;

	StateHandler state_handler_;
};

#endif // _SCENE_APP_H
