#include "SphereColliderGjk.h"

glm::vec3 SphereColliderGjk::FindFurthestPoint(const glm::vec3& direction) const
{
	return origin + glm::normalize(direction) * radius;
}