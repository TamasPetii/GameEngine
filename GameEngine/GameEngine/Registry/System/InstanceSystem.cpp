#include "InstanceSystem.h"

void InstanceSystem::OnStart(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager)
{

}

void InstanceSystem::OnUpdate(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager)
{
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto materialPool = registry->GetComponentPool<MaterialComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	manager->GetGeometry("Cube")->ClearInstances();
	manager->GetGeometry("Pyramid")->ClearInstances();
	manager->GetGeometry("Cylinder")->ClearInstances();
	manager->GetGeometry("Torus")->ClearInstances();
	manager->GetGeometry("Sphere")->ClearInstances();
	manager->GetGeometry("Cone")->ClearInstances();

	std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) &&
				materialPool->HasComponent(entity) &&
				shapePool->GetComponent(entity).shape != nullptr)
			{
				auto& transformComponent = transformPool->GetComponent(entity);
				auto& shapeComponent = shapePool->GetComponent(entity);
				auto transformIndex = transformPool->GetIndex(entity);
				auto materialIndex = materialPool->GetIndex(entity);

				if (shapeComponent.isInstanced && shapeComponent.toRender)
					shapeComponent.shape->AddInstanceID(glm::uvec4(entity, transformIndex, materialIndex, 0));
			}
		}
	);

	manager->GetGeometry("Cube")->UpdateInstanceUbo();
	manager->GetGeometry("Pyramid")->UpdateInstanceUbo();
	manager->GetGeometry("Cylinder")->UpdateInstanceUbo();
	manager->GetGeometry("Torus")->UpdateInstanceUbo();
	manager->GetGeometry("Sphere")->UpdateInstanceUbo();
	manager->GetGeometry("Cone")->UpdateInstanceUbo();
}