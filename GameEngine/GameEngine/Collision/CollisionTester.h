#pragma once
#include "Collider.h"
#include "Simplex.h"
#include <Registry/Component/DefaultCollider.h>
#include <Registry/Component/SphereCollider.h>

class CollisionTester
{
public:
	static bool Test(const Collider* colliderA, const Collider* colliderB, Simplex& simplex);
private:
	static bool SameDirection(const glm::vec3& direction, const glm::vec3& ao);
	static bool NextSimplex(Simplex& simplex, glm::vec3& direction);
	static bool LineCase(Simplex& simplex, glm::vec3& direction);
	static bool TriangleCase(Simplex& simplex, glm::vec3& direction);
	static bool TetrahedronCase(Simplex& simplex, glm::vec3& direction);
	static glm::vec3 Support(const Collider* colliderA, const Collider* colliderB, const glm::vec3& direction);
};

