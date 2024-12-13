#pragma once
#include "EngineApi.h"
#include <IrrKlang/irrKlang.h>
#include <memory>

struct ENGINE_API AudioComponent
{
	AudioComponent();

	bool startSound;
	bool isLooped;
	bool isPaused;
	float speed;
	float volume;
	float minDist;
	float maxDist;
	std::shared_ptr<irrklang::ISoundSource> soundSource;
	std::shared_ptr<irrklang::ISound> sound;
};

