#include "InstanceSystem.h"

void InstanceSystem::OnStart(std::shared_ptr<Registry> registry)
{

}

void InstanceSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	GeometryInstances(registry);
	ModelInstances(registry);
}

void InstanceSystem::GeometryInstances(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto materialPool = registry->GetComponentPool<MaterialComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	if (!transformPool || !materialPool || !shapePool)
		return;

	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;
		geometry->ClearInstances();
	}

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

	if (!transformPool || !materialPool || !modelPool)
		return;

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
}