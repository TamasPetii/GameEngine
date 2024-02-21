#include "BvhSystem.h"

void BvhSystem::OnStart(std::shared_ptr<Registry> registry)
{

}

void BvhSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto bvhTransformSsbo = resourceManager->GetSsbo("BvhTransformData");
	auto bvhTransformSsboHandler = static_cast<glm::mat4*>(bvhTransformSsbo->MapBuffer(GL_WRITE_ONLY));
	auto defaultColliderPool = registry->GetComponentPool<DefaultCollider>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	{ //Dynamic Bvh
		auto start = std::chrono::high_resolution_clock::now();

		if (bvh != nullptr)
			delete bvh;
		bvh = new Bvh();
	
		std::for_each(std::execution::seq, defaultColliderPool->GetDenseEntitiesArray().begin(), defaultColliderPool->GetDenseEntitiesArray().end(),
			[&](const Entity& entity) -> void {
				if (transformPool->HasComponent(entity) &&
					shapePool->HasComponent(entity) &&
					shapePool->GetComponent(entity).shape != nullptr)
				{
					auto& defaultColliderComponent = defaultColliderPool->GetComponent(entity);
					bvh->Insert(entity, defaultColliderComponent);
				}
			}
		);

		auto end = std::chrono::high_resolution_clock::now();
		//std::cout << "BvhSystem = " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;

		std::queue<BvhNode*> queue;
		queue.push(bvh->root);
		index = 0;
		while (!queue.empty())
		{
			BvhNode* currentNode = queue.front();
			queue.pop();

			glm::vec3 origin = 0.5f * (currentNode->aabb.max + currentNode->aabb.min);
			glm::vec3 extents = 0.5f * (currentNode->aabb.max - currentNode->aabb.min);
			bvhTransformSsboHandler[index] = glm::translate(origin) * glm::scale(extents);

			if (currentNode->left)
				queue.push(currentNode->left);
			if (currentNode->right)
				queue.push(currentNode->right);

			index++;
		}

		bvhTransformSsbo->UnMapBuffer();
	}
	
	/*
	{
		//auto start = std::chrono::high_resolution_clock::now();
		std::vector<std::pair<Entity, AABB>> aabbs;

		std::for_each(std::execution::seq, defaultColliderPool->GetDenseEntitiesArray().begin(), defaultColliderPool->GetDenseEntitiesArray().end(),
			[&](const Entity& entity) -> void {
				if (transformPool->HasComponent(entity) &&
					shapePool->HasComponent(entity) &&
					shapePool->GetComponent(entity).shape != nullptr)
				{
					auto& defaultColliderComponent = defaultColliderPool->GetComponent(entity);
					aabbs.emplace_back(std::make_pair(entity, AABB(defaultColliderComponent.aabbMax, defaultColliderComponent.aabbMin)));
				}
			}
		);
		
		if (bvhStatic != nullptr)
			delete bvhStatic;

		bvhStatic = new BvhStatic(aabbs.begin(), aabbs.end());

		//auto end = std::chrono::high_resolution_clock::now();
		//std::cout << "BvhSystem = " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;

		std::queue<BvhNode*> queue;
		queue.push(bvhStatic->root);
		index = 0;
		while (!queue.empty())
		{
			BvhNode* currentNode = queue.front();
			queue.pop();

			glm::vec3 origin = 0.5f * (currentNode->aabb.max + currentNode->aabb.min);
			glm::vec3 extents = 0.5f * (currentNode->aabb.max - currentNode->aabb.min);
			bvhTransformSsboHandler[index] = glm::translate(origin) * glm::scale(extents);

			if (currentNode->left)
				queue.push(currentNode->left);
			if (currentNode->right)
				queue.push(currentNode->right);

			index++;
		}

		bvhTransformSsbo->UnMapBuffer();
	}
	*/
}