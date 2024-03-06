#include "CollisionSystem.h"

/*
int              CollisionSystem::naiveCounter = 0;
std::atomic<int> CollisionSystem::aabbCounter = 0;
std::atomic<int> CollisionSystem::gjkCounter = 0;
std::atomic<int> CollisionSystem::gjkSuccess = 0;
*/

void CollisionSystem::OnStart(std::shared_ptr<Registry> registry)
{

}

void CollisionSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	collisionData.clear();

	auto defaultColliderPool = registry->GetComponentPool<DefaultCollider>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	//auto bvh = BvhSystem::bvh;
	auto bvh = BvhSystem::bvh;
	std::mutex mutex;
	aabbCounter = 0;
	gjkCounter = 0;
	gjkSuccess = 0;
	naiveCounter = defaultColliderPool->GetDenseEntitiesArray().size() * (defaultColliderPool->GetDenseEntitiesArray().size() + 1) / 2;

	std::for_each(std::execution::par, defaultColliderPool->GetDenseEntitiesArray().begin(), defaultColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) &&
				shapePool->HasComponent(entity) &&
				shapePool->GetComponent(entity).shape != nullptr)
			{
				auto& defaultColliderComponent = defaultColliderPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				auto& shapeComponent = shapePool->GetComponent(entity);

				AABB defaultAABB = { defaultColliderComponent.aabbMax, defaultColliderComponent.aabbMin };
				std::queue<BvhNode*> queue;

				if (AABB::Test(defaultAABB, bvh->GetRoot()->aabb))
					queue.push(bvh->GetRoot());

				while (!queue.empty())
				{
					BvhNode* currentNode = queue.front();
					queue.pop();

					if (currentNode->left)
					{
						if (AABB::Test(defaultAABB, currentNode->left->aabb))
							queue.push(currentNode->left);
						++aabbCounter; // +1 AABB test
					}

					if (currentNode->right)
					{
						if (AABB::Test(defaultAABB, currentNode->right->aabb))
							queue.push(currentNode->right);
						++aabbCounter; // +1 AABB test
					}

					if (currentNode->isLeaf && currentNode->entity != entity)
					{
						++gjkCounter; // +1 GJK test

						Simplex simplex;
						auto& otherDefaultCollider = defaultColliderPool->GetComponent(currentNode->entity);
						if (CollisionTester::Test(&defaultColliderComponent, &otherDefaultCollider, simplex))
						{
							gjkSuccess++;
							std::lock_guard<std::mutex> lock(mutex);
							collisionData.push_back(std::make_tuple(entity, currentNode->entity, simplex));
						}
					}
				}
			}
		}
	);
}