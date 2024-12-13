#pragma once
#include "EngineApi.h"
#include <glm/glm.hpp>

struct ENGINE_API AABB
{
	AABB() = default;
	AABB(const glm::vec3& max, const glm::vec3& min);
	static bool Test(const AABB& colliderA, const AABB& colliderB);
	static bool Test(const glm::vec3& aabbMinA, const glm::vec3& aabbMaxA, const glm::vec3& aabbMinB, const glm::vec3& aabbMaxB);

	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 origin;
};

