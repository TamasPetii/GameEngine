#pragma once
#include "EngineApi.h"
#include <glm/glm.hpp>

struct ENGINE_API AABB
{
	AABB() = default;
	AABB(const glm::vec3& max, const glm::vec3& min) : max(max), min(min), origin((max + min) / 2.f) {}
	static bool Test(const AABB& colliderA, const AABB& colliderB);

	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 origin;
};

