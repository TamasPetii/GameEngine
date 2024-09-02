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
	auto modelPool = registry->GetComponentPool<ModelComponent>();

	if (!defaultColliderPool || !transformPool)
		return;

	static glm::mat4* dcTransformSsboHandler = nullptr;

	if (!dcTransformSsboHandler)
	{
		auto dcTransformSsbo = resourceManager->GetSsbo("DefaultColliderTransform");
		dcTransformSsboHandler = static_cast<glm::mat4*>(dcTransformSsbo->MapBufferRange());
	}


	glm::vec3 defaultOrigin(0);
	glm::vec3 defaultExtents(0);
	std::array<glm::vec3, 8> defaultObb;

	std::for_each(std::execution::par, defaultColliderPool->GetDenseEntitiesArray().begin(), defaultColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			bool isShape = shapePool && shapePool->HasComponent(entity) && shapePool->GetComponent(entity).shape != nullptr;
			bool isModel = modelPool && modelPool->HasComponent(entity) && modelPool->GetComponent(entity).model != nullptr;
			bool shapeChanged = isShape && shapePool->IsFlagSet(entity, CHANGED_FLAG);
			bool modelChanged = isModel && modelPool->IsFlagSet(entity, CHANGED_FLAG);
			if (transformPool->HasComponent(entity) && (defaultColliderPool->IsFlagSet(entity, UPDATE_FLAG) || transformPool->IsFlagSet(entity, CHANGED_FLAG) || modelChanged || shapeChanged))
			{
				if (isShape || isModel)
				{
					auto& defaultColliderComponent = defaultColliderPool->GetComponent(entity);
					auto& transformComponent = transformPool->GetComponent(entity);
					auto indexDC = defaultColliderPool->GetIndex(entity);

					glm::vec3 origin = isShape ? shapePool->GetComponent(entity).shape->GetObbOrigin()
									 : isModel ? modelPool->GetComponent(entity).model->GetObbOrigin()
									 : defaultOrigin;

					glm::vec3 extents = isShape ? shapePool->GetComponent(entity).shape->GetObbExtents()
									  : isModel ? modelPool->GetComponent(entity).model->GetObbExtents()
									  : defaultExtents;

					std::array<glm::vec3, 8> obb = isShape ? shapePool->GetComponent(entity).shape->GetObb()
												 : isModel ? modelPool->GetComponent(entity).model->GetObb()
												 : defaultObb;

					glm::vec3 maxPosition{ std::numeric_limits<float>::lowest() };
					glm::vec3 minPosition{ std::numeric_limits<float>::max() };

					for (unsigned int i = 0; i < 8; ++i)
					{
						glm::vec4 point = transformComponent.fullTransform * glm::vec4(obb[i], 1);
						defaultColliderComponent.positions[i] = glm::vec3(point);

						if (defaultColliderComponent.positions[i].x > maxPosition.x) maxPosition.x = defaultColliderComponent.positions[i].x;
						if (defaultColliderComponent.positions[i].y > maxPosition.y) maxPosition.y = defaultColliderComponent.positions[i].y;
						if (defaultColliderComponent.positions[i].z > maxPosition.z) maxPosition.z = defaultColliderComponent.positions[i].z;
						if (defaultColliderComponent.positions[i].x < minPosition.x) minPosition.x = defaultColliderComponent.positions[i].x;
						if (defaultColliderComponent.positions[i].y < minPosition.y) minPosition.y = defaultColliderComponent.positions[i].y;
						if (defaultColliderComponent.positions[i].z < minPosition.z) minPosition.z = defaultColliderComponent.positions[i].z;
					}

					defaultColliderComponent.aabbMin = minPosition;
					defaultColliderComponent.aabbMax = maxPosition;
					defaultColliderComponent.origin = transformComponent.fullTransform * glm::vec4(origin, 1);

					glm::mat4 dcTransform = transformComponent.fullTransform * glm::translate(origin) * glm::scale(extents);
					dcTransformSsboHandler[indexDC] = dcTransform;
					defaultColliderPool->ResFlag(entity, UPDATE_FLAG);
				}
			}
		}
	);

	//dcTransformSsbo->UnMapBuffer();
}