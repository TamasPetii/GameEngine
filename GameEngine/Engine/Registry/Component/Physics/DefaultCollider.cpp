#include "DefaultCollider.h"

glm::vec3 DefaultCollider::FindFurthestPoint(const glm::vec3& direction) const
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