#include "SphereCollider.h"

glm::vec3 SphereCollider::FindFurthestPoint(const glm::vec3& direction) const
{
	return origin + glm::normalize(direction) * radius;
}