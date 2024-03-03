#include "KdBvh.h"

KdBvh::KdBvh(std::vector<std::pair<Entity, AABB>>::iterator begin, std::vector<std::pair<Entity, AABB>>::iterator end)
{
	root = BuildStaticBvh(begin, end);
}

KdBvh::~KdBvh()
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

void KdBvh::Insert(Entity entity, AABB& aabb)
{
	BvhNode* insertNode = new BvhNode();
	insertNode->isLeaf = true;
	insertNode->entity = entity;
	insertNode->aabb = aabb;

	BvhNode* currentNode = root;
	while (currentNode != nullptr)
	{
		if (currentNode->left == nullptr)
		{
			insertNode->parent = currentNode;
			currentNode->left = insertNode;

			if (currentNode->right == nullptr)
				currentNode->aabb = currentNode->left->aabb;
			else
			{

				currentNode->aabb = CalculateAABB(currentNode->left, currentNode->right);
			}

			currentNode = nullptr;
			continue;
		}
		else if (currentNode->right == nullptr)
		{
			insertNode->parent = currentNode;
			currentNode->right = insertNode;

			if (currentNode->left == nullptr)
				currentNode->aabb = currentNode->right->aabb;
			else
				currentNode->aabb = CalculateAABB(currentNode->left, currentNode->right);

			currentNode = nullptr;
			continue;
		}
		else
		{
			float svh1 = SurfaceVolumeHeuristic(insertNode, currentNode->left);
			float svh2 = SurfaceVolumeHeuristic(insertNode, currentNode->right);

			if (svh1 <= svh2)
			{
				if (!currentNode->left->isLeaf)
				{
					currentNode = currentNode->left;
					continue;
				}
				else
				{
					BvhNode* parent = new BvhNode();
					parent->aabb = CalculateAABB(insertNode, currentNode->left);
					parent->entity = null;
					parent->isLeaf = false;
					parent->left = currentNode->left;
					parent->right = insertNode;
					parent->parent = currentNode->left->parent;

					currentNode->left->parent = parent;
					insertNode->parent = parent;
					currentNode->left = parent;

					RefreshParent(currentNode);

					currentNode = nullptr;
					continue;
				}
			}
			else if (svh2 < svh1)
			{
				if (!currentNode->right->isLeaf)
				{
					currentNode = currentNode->right;
					continue;
				}
				else
				{
					BvhNode* parent = new BvhNode();
					parent->aabb = CalculateAABB(insertNode, currentNode->right);
					parent->entity = null;
					parent->isLeaf = false;
					parent->left = currentNode->right;
					parent->right = insertNode;
					parent->parent = currentNode->right->parent;

					currentNode->right->parent = parent;
					insertNode->parent = parent;
					currentNode->right = parent;

					RefreshParent(currentNode);

					currentNode = nullptr;
					continue;
				}
			}
		}
	}
}

void KdBvh::RefreshParent(BvhNode* node)
{
	while (node != nullptr)
	{
		node->aabb = CalculateAABB(node->left, node->right);
		node = node->parent;
	}
}

float KdBvh::SurfaceVolumeHeuristic(BvhNode* nodeA, BvhNode* nodeB)
{
	AABB aabb = CalculateAABB(nodeA, nodeB);
	glm::vec3 d = aabb.max - aabb.min;
	return 2.0f * (d.x * d.y + d.y * d.z + d.z * d.x);
}

AABB KdBvh::CalculateAABB(BvhNode* nodeA, BvhNode* nodeB)
{
	glm::vec3 min = glm::min(nodeA->aabb.min, nodeB->aabb.min);
	glm::vec3 max = glm::max(nodeA->aabb.max, nodeB->aabb.max);
	return { max, min };
}

BvhNode* KdBvh::BuildStaticBvh(std::vector<std::pair<Entity, AABB>>::iterator begin, std::vector<std::pair<Entity, AABB>>::iterator end)
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
	float minAxis = glm::max(glm::max(distX, distY), distZ);
	int axis = glm::abs(minAxis - distX) < 0.001 ? 0 : glm::abs(minAxis - distY) < 0.001 ? 1 : 2;

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
	node->left = BuildStaticBvh(begin, middle);
	node->right = BuildStaticBvh(middle, end);
	node->parent = nullptr;

	return node;
}
