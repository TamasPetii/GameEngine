#pragma once
#include "EngineApi.h"
#include <glm/glm.hpp>
#include <Registry/Component/Component.h>
#include <vector>
#include <Collision/Collider.h>

struct ENGINE_API MeshCollider : public Collider, public Component
{
	std::vector<glm::vec3> positions;
	glm::vec3 FindFurthestPoint(const glm::vec3& direction) const override;
};