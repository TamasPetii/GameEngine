#include "AABB.h"

bool AABB::Test(const AABB& colliderA, const AABB& colliderB)
{
	return (
		colliderA.min.x <= colliderB.max.x &&
		colliderA.max.x >= colliderB.min.x &&
		colliderA.min.y <= colliderB.max.y &&
		colliderA.max.y >= colliderB.min.y &&
		colliderA.min.z <= colliderB.max.z &&
		colliderA.max.z >= colliderB.min.z
		);
}

bool AABB::Test(const glm::vec3& aabbMinA, const glm::vec3& aabbMaxA, const glm::vec3& aabbMinB, const glm::vec3& aabbMaxB)
{
	return (
		aabbMinA.x <= aabbMaxB.x &&
		aabbMaxA.x >= aabbMinB.x &&
		aabbMinA.y <= aabbMaxB.y &&
		aabbMaxA.y >= aabbMinB.y &&
		aabbMinA.z <= aabbMaxB.z &&
		aabbMaxA.z >= aabbMinB.z
		);
}