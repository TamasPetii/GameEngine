#include "InstanceSystem.h"

<<<<<<< HEAD
void InstanceSystem::OnStart(std::shared_ptr<Registry> registry)
=======
void InstanceSystem::OnStart(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager)
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
{

}

<<<<<<< HEAD
void InstanceSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	GeometryInstances(registry);
	ModelInstances(registry);
}

void InstanceSystem::GeometryInstances(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
=======
void InstanceSystem::OnUpdate(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager)
{
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto materialPool = registry->GetComponentPool<MaterialComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

<<<<<<< HEAD
	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;
		geometry->ClearInstances();
	}
=======
	manager->GetGeometry("Cube")->ClearInstances();
	manager->GetGeometry("Pyramid")->ClearInstances();
	manager->GetGeometry("Cylinder")->ClearInstances();
	manager->GetGeometry("Torus")->ClearInstances();
	manager->GetGeometry("Sphere")->ClearInstances();
	manager->GetGeometry("Cone")->ClearInstances();
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

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

<<<<<<< HEAD
	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;
		geometry->UpdateInstanceUbo();
	}
}

void InstanceSystem::ModelInstances(std::shared_ptr<Registry> registry)
{
	auto modelManager = ModelManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto materialPool = registry->GetComponentPool<MaterialComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;
		model->ClearInstances();
	}

	std::for_each(std::execution::seq, modelPool->GetDenseEntitiesArray().begin(), modelPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) &&
				modelPool->GetComponent(entity).model != nullptr)
			{
				auto& transformComponent = transformPool->GetComponent(entity);
				auto& modelComponent = modelPool->GetComponent(entity);
				auto transformIndex = transformPool->GetIndex(entity);

				if (modelComponent.isInstanced && modelComponent.toRender)
					modelComponent.model->AddInstanceID(glm::uvec4(entity, transformIndex, 0, 0));
			}
		}
	);

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;
		model->UpdateInstanceSsbo();
	}
=======
	manager->GetGeometry("Cube")->UpdateInstanceUbo();
	manager->GetGeometry("Pyramid")->UpdateInstanceUbo();
	manager->GetGeometry("Cylinder")->UpdateInstanceUbo();
	manager->GetGeometry("Torus")->UpdateInstanceUbo();
	manager->GetGeometry("Sphere")->UpdateInstanceUbo();
	manager->GetGeometry("Cone")->UpdateInstanceUbo();
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
}