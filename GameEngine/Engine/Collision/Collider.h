#pragma once
#include "EngineApi.h"
#include <glm/glm.hpp>

class ENGINE_API Collider
{
public:
	virtual ~Collider() = default;
	virtual glm::vec3 FindFurthestPoint(const glm::vec3& direction) const = 0;
};