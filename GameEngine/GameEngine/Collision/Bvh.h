#pragma once
#include <Registry/Entity.h>
#include <Registry/Component/DefaultCollider.h>
#include <glm/glm.hpp>
#include <queue>
<<<<<<< HEAD
#include "AABB.h"
=======

struct AABB
{
	AABB() = default;
	AABB(const glm::vec3& max, const glm::vec3& min) : max(max), min(min), origin((max + min) / 2.f) {}
	static bool Test(const AABB& colliderA, const AABB& colliderB);

	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 origin;
};
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

struct BvhNode
{
	bool isLeaf = false;
	AABB aabb;
	BvhNode* left = nullptr;
	BvhNode* right = nullptr;
	BvhNode* parent = nullptr;
	uint32_t entity;
};

class Bvh
{
public:
	Bvh();
	~Bvh();
	void Insert(Entity entity, const DefaultCollider& collider);
	BvhNode* root;

	static int Size(BvhNode* node);
private:
	AABB CalculateAABB(BvhNode* nodeA, BvhNode* nodeB);
	float SurfaceVolumeHeuristic(BvhNode* nodeA, BvhNode* nodeB);
	void RefreshParent(BvhNode* node);
};

