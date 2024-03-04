#pragma once
#include "EngineApi.h"
#include <string>
#include <glm/glm.hpp>
#include <Registry/Component/Component.h>

class ENGINE_API TagComponent : public Component
{
public:
	std::string name;
	std::string tag;
};
