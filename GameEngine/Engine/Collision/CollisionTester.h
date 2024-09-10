#pragma once
#include "EngineApi.h"
#include "Collider.h"
#include "Simplex.h"
#include "Logger/Logger.h"

class ENGINE_API CollisionTester
{
public:
	static bool Test(Collider* colliderA, Collider* colliderB, Simplex& simplex);
	static glm::vec3 Support(Collider* colliderA, Collider* colliderB, const glm::vec3& direction);
	static bool SameDirection(const glm::vec3& direction, const glm::vec3& ao);
private:
	static bool NextSimplex(Simplex& simplex, glm::vec3& direction);
	static bool LineCase(Simplex& simplex, glm::vec3& direction);
	static bool TriangleCase(Simplex& simplex, glm::vec3& direction);
	static bool TetrahedronCase(Simplex& simplex, glm::vec3& direction);
};

