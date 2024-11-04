#pragma once
#include "EngineApi.h"
#include <glm/glm.hpp>
#include <array>
#include <Collision/Collider.h>

struct ENGINE_API DefaultCollider : public Collider
{
	glm::vec3 origin;
	glm::vec3 aabbMin;
	glm::vec3 aabbMax;
	std::array<glm::vec3, 8> positions;
	glm::vec3 FindFurthestPoint(const glm::vec3& direction) const override;
};