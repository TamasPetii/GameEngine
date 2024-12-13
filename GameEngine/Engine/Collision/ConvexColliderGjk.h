#pragma once
#include "EngineApi.h"
#include "Collision/Collider.h"

#include <vector>
#include <glm/glm.hpp>

struct ENGINE_API ConvexColliderGjk : public Collider
{
	std::vector<glm::vec3> positions;
	glm::vec3 FindFurthestPoint(const glm::vec3& direction) const override;
};