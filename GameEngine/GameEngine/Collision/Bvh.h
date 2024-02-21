#pragma once
#include <Registry/Entity.h>
#include <Registry/Component/DefaultCollider.h>
#include <glm/glm.hpp>
#include <queue>
#include "AABB.h"

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

