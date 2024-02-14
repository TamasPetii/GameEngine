#include "Bvh.h"

bool AABB::Test(const AABB& colliderA, const AABB& colliderB)
{
	return (
		colliderA.min.x <= colliderB.max.x &&
		colliderA.max.x >= colliderB.min.x &&
		colliderA.min.y <= colliderB.max.y &&
		colliderA.max.y >= colliderB.min.y &&
		colliderA.min.z <= colliderB.max.z &&
		colliderA.max.z >= colliderB.min.z
	);
}

Bvh::Bvh()
{
	root = new BvhNode();
	root->isLeaf = false;
	root->entity = null;
}

Bvh::~Bvh()
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

void Bvh::Insert(Entity entity, const DefaultCollider& collider)
{
	BvhNode* insertNode = new BvhNode();
	insertNode->isLeaf = true;
	insertNode->entity = entity;
	insertNode->aabb = { collider.aabbMax, collider.aabbMin };

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

AABB Bvh::CalculateAABB(BvhNode* nodeA, BvhNode* nodeB)
{
	glm::vec3 min = glm::min(nodeA->aabb.min, nodeB->aabb.min);
	glm::vec3 max = glm::max(nodeA->aabb.max, nodeB->aabb.max);
	return { max, min };
}

float Bvh::SurfaceVolumeHeuristic(BvhNode* nodeA, BvhNode* nodeB)
{
	/*
	AABB aabb = CalculateAABB(nodeA, nodeB);
	glm::vec3 edge = aabb.max - aabb.min;
	return edge.x * edge.y * edge.z;
	*/

	AABB aabb = CalculateAABB(nodeA, nodeB);
	glm::vec3 d = aabb.max - aabb.min;
	return 2.0f * (d.x * d.y + d.y * d.z + d.z * d.x);
}

void Bvh::RefreshParent(BvhNode* node)
{
	while (node != nullptr)
	{
		node->aabb = CalculateAABB(node->left, node->right);
		node = node->parent;
	}
}

int Bvh::Size(BvhNode* node)
{
	std::queue<BvhNode*> queue;
	queue.push(node);
	int size = 0;
	while (!queue.empty())
	{
		BvhNode* currentNode = queue.front();
		queue.pop();
		size++;

		if (currentNode->left)
			queue.push(currentNode->left);
		if (currentNode->right)
			queue.push(currentNode->right);
	}

	return size;
}