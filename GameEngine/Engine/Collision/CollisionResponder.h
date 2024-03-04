#pragma once
#include "Simplex.h"
#include "Collider.h"
#include "CollisionTester.h"

struct CollisionRespond
{
	float depth;
	glm::vec3 normal;
};

class CollisionResponder
{
public:
	static CollisionRespond Resolve(Collider* colliderA, Collider* colliderB, Simplex& simplex);
private:
	static void AddIfUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b);
	static std::pair<std::vector<glm::vec4>, size_t> GetFaceNormals(const std::vector<glm::vec3>& polytope, const std::vector<size_t>& faces);
};

