#include "DefaultColliderSystem.h"

void DefaultColliderSystem::OnStart(std::shared_ptr<Registry> registry)
{

}

void DefaultColliderSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto defaultColliderPool = registry->GetComponentPool<DefaultCollider>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	static bool init = true;
	static glm::mat4* dcTransformSsboHandler = nullptr;
	if (init)
	{
		init = false;
		auto dcTransformSsbo = resourceManager->GetSsbo("DefaultColliderTransform");
		dcTransformSsboHandler = static_cast<glm::mat4*>(dcTransformSsbo->MapBuffer(GL_WRITE_ONLY));
	}

	std::for_each(std::execution::par, defaultColliderPool->GetDenseEntitiesArray().begin(), defaultColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) && 
				shapePool->HasComponent(entity) &&
				shapePool->GetComponent(entity).shape != nullptr &&
				(defaultColliderPool->IsFlagSet(entity, UPDATE_FLAG) || transformPool->IsFlagSet(entity, CHANGED_FLAG)))
			{
				auto& defaultCollderComponent = defaultColliderPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				auto& shapeComponent = shapePool->GetComponent(entity);
				auto indexDC = defaultColliderPool->GetIndex(entity);

				glm::vec3 maxPosition{ std::numeric_limits<float>::lowest() };
				glm::vec3 minPosition{ std::numeric_limits<float>::max() };

				for (unsigned int i = 0; i < 8; ++i)
				{
					glm::vec4 point = transformComponent.fullTransform * glm::vec4(shapeComponent.shape->GetObb()[i], 1);
					defaultCollderComponent.positions[i] = glm::vec3(point);

					if (defaultCollderComponent.positions[i].x > maxPosition.x) maxPosition.x = defaultCollderComponent.positions[i].x;
					if (defaultCollderComponent.positions[i].y > maxPosition.y) maxPosition.y = defaultCollderComponent.positions[i].y;
					if (defaultCollderComponent.positions[i].z > maxPosition.z) maxPosition.z = defaultCollderComponent.positions[i].z;
					if (defaultCollderComponent.positions[i].x < minPosition.x) minPosition.x = defaultCollderComponent.positions[i].x;
					if (defaultCollderComponent.positions[i].y < minPosition.y) minPosition.y = defaultCollderComponent.positions[i].y;
					if (defaultCollderComponent.positions[i].z < minPosition.z) minPosition.z = defaultCollderComponent.positions[i].z;
				}

				defaultCollderComponent.aabbMin = minPosition;
				defaultCollderComponent.aabbMax = maxPosition;
				defaultCollderComponent.origin = transformComponent.fullTransform * glm::vec4(shapeComponent.shape->GetObbOrigin(), 1);

				glm::mat4 dcTransform = transformComponent.fullTransform * glm::translate(shapeComponent.shape->GetObbOrigin()) * glm::scale(shapeComponent.shape->GetObbExtents());		
				dcTransformSsboHandler[indexDC] = dcTransform;
				defaultColliderPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);

	//dcTransformSsbo->UnMapBuffer();
}