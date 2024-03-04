#pragma once
#include "EngineApi.h"
#include <glm/glm.hpp>
#include <Registry/Component/Component.h>
#include <array>
#include <Collision/Collider.h>

struct ENGINE_API DefaultCollider : public Collider , public Component
{
	glm::vec3 origin;
	glm::vec3 aabbMin;
	glm::vec3 aabbMax;
	std::array<glm::vec3, 8> positions;
	inline glm::vec3 FindFurthestPoint(const glm::vec3& direction) const override;
};

inline glm::vec3 DefaultCollider::FindFurthestPoint(const glm::vec3& direction) const
{
	glm::vec3 maxPosition(0.0f, 0.0f, 0.0f);
	float maxDistance = std::numeric_limits<float>::lowest();

	for (auto& position : positions)
	{
		float distance = glm::dot(position, direction);

		if (distance > maxDistance)
		{
			maxDistance = distance;
			maxPosition = position;
		}
	}

	return maxPosition;
}