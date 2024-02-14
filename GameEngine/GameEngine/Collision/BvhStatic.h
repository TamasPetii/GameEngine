#pragma once
#include "Bvh.h"
#include <iostream>
#include <queue>
#include <stack>

class BvhStatic
{
public:
	BvhStatic(std::vector<std::pair<Entity, AABB>>::iterator begin, std::vector<std::pair<Entity, AABB>>::iterator end);
	~BvhStatic();
	BvhNode* root = nullptr;
private:
	BvhNode* BuildBvh(std::vector<std::pair<Entity, AABB>>::iterator begin, std::vector<std::pair<Entity, AABB>>::iterator end);
};

