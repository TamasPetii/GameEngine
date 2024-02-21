#include "MeshColliderSystem.h"

<<<<<<< HEAD
void MeshColliderSystem::OnStart(std::shared_ptr<Registry> registry)
=======
void MeshColliderSystem::OnStart(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager)
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
{

}

<<<<<<< HEAD
void MeshColliderSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
=======
void MeshColliderSystem::OnUpdate(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager)
{
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	auto meshColliderPool = registry->GetComponentPool<MeshCollider>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	std::for_each(std::execution::par, meshColliderPool->GetDenseEntitiesArray().begin(), meshColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) &&
				shapePool->HasComponent(entity) &&
				shapePool->GetComponent(entity).shape != nullptr &&
				(meshColliderPool->IsFlagSet(entity, UPDATE_FLAG) || transformPool->IsFlagSet(entity, CHANGED_FLAG)))
			{
				auto& meshColliderComponent = meshColliderPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				auto& shapeComponent = shapePool->GetComponent(entity);

				meshColliderComponent.positions.clear();
				meshColliderComponent.positions.reserve(shapeComponent.shape->GetSurfacePoints().size());

				for (const auto& position : shapeComponent.shape->GetSurfacePoints())
					meshColliderComponent.positions.emplace_back(transformComponent.fullTransform * glm::vec4(position, 1));

				meshColliderPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);
}