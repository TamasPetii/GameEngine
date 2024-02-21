#pragma once
#include "Collider.h"
#include "Simplex.h"
#include <Registry/Component/DefaultCollider.h>
#include <Registry/Component/SphereCollider.h>

class CollisionTester
{
public:
<<<<<<< HEAD
	static bool Test(Collider* colliderA, Collider* colliderB, Simplex& simplex);
	static glm::vec3 Support(Collider* colliderA, Collider* colliderB, const glm::vec3& direction);
	static bool SameDirection(const glm::vec3& direction, const glm::vec3& ao);
private:
=======
	static bool Test(const Collider* colliderA, const Collider* colliderB, Simplex& simplex);
private:
	static bool SameDirection(const glm::vec3& direction, const glm::vec3& ao);
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	static bool NextSimplex(Simplex& simplex, glm::vec3& direction);
	static bool LineCase(Simplex& simplex, glm::vec3& direction);
	static bool TriangleCase(Simplex& simplex, glm::vec3& direction);
	static bool TetrahedronCase(Simplex& simplex, glm::vec3& direction);
<<<<<<< HEAD
=======
	static glm::vec3 Support(const Collider* colliderA, const Collider* colliderB, const glm::vec3& direction);
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
};

