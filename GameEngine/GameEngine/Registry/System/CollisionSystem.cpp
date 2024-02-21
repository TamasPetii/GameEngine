#include "CollisionSystem.h"

<<<<<<< HEAD
void CollisionSystem::OnStart(std::shared_ptr<Registry> registry)
=======
void CollisionSystem::OnStart(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager)
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
{

}

<<<<<<< HEAD
void CollisionSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	collisionData.clear();
=======
void CollisionSystem::OnUpdate(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager)
{
	//auto start = std::chrono::high_resolution_clock::now();
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

	auto defaultColliderPool = registry->GetComponentPool<DefaultCollider>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

<<<<<<< HEAD
	auto bvh = BvhSystem::bvh;
	//auto bvh = BvhSystem::bvhStatic;
	std::mutex mutex;
	aabbCounter = 0;
	gjkCounter = 0;
	gjkSuccess = 0;
	naiveCounter = defaultColliderPool->GetDenseEntitiesArray().size() * (defaultColliderPool->GetDenseEntitiesArray().size() + 1) / 2;
=======
	//auto bvh = BvhSystem::bvh;
	auto bvh = BvhSystem::bvhStatic;
	std::atomic<int> aabbCounter = 0;
	std::atomic<int> gjkCounter = 0;
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

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

				if (AABB::Test(defaultAABB, bvh->root->aabb))
					queue.push(bvh->root);

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
<<<<<<< HEAD

						Simplex simplex;
						auto& otherDefaultCollider = defaultColliderPool->GetComponent(currentNode->entity);
						if (CollisionTester::Test(&defaultColliderComponent, &otherDefaultCollider, simplex))
						{
							gjkSuccess++;
							std::lock_guard<std::mutex> lock(mutex);
							collisionData.push_back(std::make_tuple(entity, currentNode->entity, simplex));
						}
=======
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
					}
				}
			}
		}
	);
<<<<<<< HEAD
=======

	//auto end = std::chrono::high_resolution_clock::now();
	//std::cout << "NAIVE = " << 10127250 << std::endl;
	//std::cout << "AABB = " << aabbCounter << std::endl;
	//std::cout << "GJK = " << gjkCounter << std::endl;
	//std::cout << "CollisionSystem = " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
}