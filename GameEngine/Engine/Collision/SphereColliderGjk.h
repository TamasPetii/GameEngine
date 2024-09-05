#pragma once

#include "EngineApi.h"
#include <glm/glm.hpp>
#include <Collision/Collider.h>

struct ENGINE_API SphereColliderGjk : public Collider
{
	float radius;
	glm::vec3 origin;
	glm::vec3 FindFurthestPoint(const glm::vec3& direction) const override;
};