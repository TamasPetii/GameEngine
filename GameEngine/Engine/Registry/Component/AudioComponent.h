#pragma once
#include "EngineApi.h"
#include <memory>
#include <map>
#include <glm/glm.hpp>
#include <IrrKlang/irrKlang.h>

class ENGINE_API AudioComponent
{
public:
	bool startSound{ false };
	bool isLooped{ false };
	bool isPaused{ false };
	float speed{ 1.f };
	float volume{ 1.f };
	float minDist{ 5.f };
	float maxDist{ 100.f };
	std::shared_ptr<irrklang::ISoundSource> soundSource = nullptr;
	std::shared_ptr<irrklang::ISound> sound = nullptr;
};
