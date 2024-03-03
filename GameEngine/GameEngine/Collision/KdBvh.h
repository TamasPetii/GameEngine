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

class KdBvh
{
public:
	KdBvh(std::vector<std::pair<Entity, AABB>>::iterator begin, std::vector<std::pair<Entity, AABB>>::iterator end);
	~KdBvh();
	void Insert(Entity entity, AABB& aabb);
	auto GetRoot() { return root; }
private:
	BvhNode* root = nullptr;
	void RefreshParent(BvhNode* node);
	float SurfaceVolumeHeuristic(BvhNode* nodeA, BvhNode* nodeB);
	AABB CalculateAABB(BvhNode* nodeA, BvhNode* nodeB);
	BvhNode* BuildStaticBvh(std::vector<std::pair<Entity, AABB>>::iterator begin, std::vector<std::pair<Entity, AABB>>::iterator end);
};