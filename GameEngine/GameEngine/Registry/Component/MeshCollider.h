#pragma once
#include <glm/glm.hpp>
#include <Registry/Component/Component.h>
#include <vector>
#include <Collision/Collider.h>

struct MeshCollider : public Collider, public Component
{
	std::vector<glm::vec3> positions;
	inline glm::vec3 FindFurthestPoint(const glm::vec3& direction) const override;
};

inline glm::vec3 MeshCollider::FindFurthestPoint(const glm::vec3& direction) const
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