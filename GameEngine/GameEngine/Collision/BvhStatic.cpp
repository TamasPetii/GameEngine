#include "BvhStatic.h"

BvhStatic::BvhStatic(std::vector<std::pair<Entity, AABB>>::iterator begin, std::vector<std::pair<Entity, AABB>>::iterator end)
{
	root = BuildBvh(begin, end);
}

BvhStatic::~BvhStatic()
{
	std::queue<BvhNode*> queue;
	queue.push(root);
	while (!queue.empty())
	{
		BvhNode* currentNode = queue.front();
		queue.pop();

		if (currentNode->left)
			queue.push(currentNode->left);
		if (currentNode->right)
			queue.push(currentNode->right);

		delete currentNode;
	}
}

BvhNode* BvhStatic::BuildBvh(std::vector<std::pair<Entity, AABB>>::iterator begin, std::vector<std::pair<Entity, AABB>>::iterator end)
{
	if (std::distance(begin, end) == 1)
	{
		BvhNode* node = new BvhNode();
		node->isLeaf = true;
		node->entity = (*begin).first;
		node->aabb = (*begin).second;
		node->left = nullptr;
		node->right = nullptr;
		node->parent = nullptr;
		return node;
	}

	glm::vec3 max{ std::numeric_limits<float>::lowest() };
	glm::vec3 min{ std::numeric_limits<float>::max() };

	std::for_each(begin, end,
		[&](const auto& data) -> void {
			max = glm::max(max, data.second.max);
			min = glm::min(min, data.second.min);
		}
	);

	AABB aabb = { max, min };
	float distX = max.x - min.x;
	float distY = max.y - min.y;
	float distZ = max.z - min.z;
	float minAxis = glm::min(glm::min(distX, distY), distZ);
	int axis = minAxis == distX ? 0 : minAxis == distY ? 1 : 2;

	std::sort(begin, end,
		[&](const auto& a, const auto& b) -> bool {
			return a.second.min[axis] < b.second.min[axis];
		}
	);

	auto middle = begin + std::distance(begin, end) / 2;

	BvhNode* node = new BvhNode();
	node->isLeaf = false;
	node->entity = null;
	node->aabb = aabb;
	node->left = BuildBvh(begin, middle);
	node->right = BuildBvh(middle, end);
	node->parent = nullptr;

	return node;
}