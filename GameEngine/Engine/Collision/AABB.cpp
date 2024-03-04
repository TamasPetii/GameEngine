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