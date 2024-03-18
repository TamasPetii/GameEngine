#include "BvhSystem.h"

int BvhSystem::index = 0;
KdBvh* BvhSystem::bvh = nullptr;

void BvhSystem::OnStart(std::shared_ptr<Registry> registry)
{

}

void BvhSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto defaultColliderPool = registry->GetComponentPool<DefaultCollider>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	if (!defaultColliderPool || !transformPool || !shapePool)
		return;

	static bool init = true;
	static glm::mat4* bvhTransformSsboHandler = nullptr;
	if (init)
	{
		init = false;
		auto bvhTransformSsbo = resourceManager->GetSsbo("BvhTransformData");
		bvhTransformSsboHandler = static_cast<glm::mat4*>(bvhTransformSsbo->MapBuffer(GL_WRITE_ONLY));
	}
	
	std::vector<std::pair<Entity, AABB>> smallAABB;
	std::vector<std::pair<Entity, AABB>> largeAABB;
	std::for_each(std::execution::seq, defaultColliderPool->GetDenseEntitiesArray().begin(), defaultColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) &&
				shapePool->HasComponent(entity) &&
				shapePool->GetComponent(entity).shape != nullptr)
			{
				auto& defaultColliderComponent = defaultColliderPool->GetComponent(entity);

				//if(glm::distance(defaultColliderComponent.aabbMax, defaultColliderComponent.aabbMin) < 7.5)
					smallAABB.emplace_back(std::make_pair(entity, AABB(defaultColliderComponent.aabbMax, defaultColliderComponent.aabbMin)));
				//else
					//largeAABB.emplace_back(std::make_pair(entity, AABB(defaultColliderComponent.aabbMax, defaultColliderComponent.aabbMin)));
			}
		}
	);
		
	if (bvh != nullptr)
		delete bvh;

	//Build bvh 
	bvh = new KdBvh(smallAABB.begin(), smallAABB.end());

	std::queue<BvhNode*> queue;
	queue.push(bvh->GetRoot());
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

	//bvhTransformSsbo->UnMapBuffer();
}