#pragma once
#include "EngineApi.h"
#include "Registry/Component/Component.h"
#include <memory>
#include <glm/glm.hpp>
#include <IrrKlang/irrKlang.h>

class ENGINE_API AudioComponent : public Component
{
public:
	bool isLooped{false};
	bool isPaused{false};
	float speed{1.f};
	float volume{1.f};
	float minDist{ 5.f };
	float maxDist{ 100.f };
	std::shared_ptr<irrklang::ISoundSource> soundSource;
	std::shared_ptr<irrklang::ISound> sound;
};