#pragma once
#include <glm/glm.hpp>
#include <Registry/Component/Component.h>
#include <array>
#include <Collision/Collider.h>

struct SphereCollider : public Collider, public Component
{
	glm::vec3 origin;
	float radius;
	inline glm::vec3 FindFurthestPoint(const glm::vec3& direction) const override;
};

inline glm::vec3 SphereCollider::FindFurthestPoint(const glm::vec3& direction) const
{
	return origin + glm::normalize(direction) * radius;
}