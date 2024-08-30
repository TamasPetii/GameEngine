#pragma once

#include "EngineApi.h"
#include <glm/glm.hpp>
#include <Registry/Component/Component.h>
#include <array>
#include <Collision/Collider.h>

struct ENGINE_API SphereCollider : public Collider, public Component
{
	glm::vec3 origin;
	float radius;
	glm::vec3 FindFurthestPoint(const glm::vec3& direction) const override;
};