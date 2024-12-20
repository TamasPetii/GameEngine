#include "InstanceSystem.h"

#include <algorithm>
#include <execution>
#include <glm/glm.hpp>

#include "Registry/Registry.h"
#include "Manager/ModelManager.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/TransformComponent.h"
#include "Registry/Component/Object/ShapeComponent.h"
#include "Registry/Component/Object/ModelComponent.h"
#include "Registry/Component/Object/MaterialComponent.h"
#include "Render/Geometry/Geometry.h"
#include "Model/Model.h"

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

	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;
		geometry->ClearInstances();
	}

	if (!transformPool || !materialPool || !shapePool)
		return;

	std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) &&
				materialPool->HasComponent(entity) &&
				shapePool->GetComponent(entity).shape != nullptr)
			{
				auto& shapeComponent = shapePool->GetComponent(entity);
				auto transformIndex = transformPool->GetIndex(entity);
				auto materialIndex = materialPool->GetIndex(entity);
				auto shapeIndex = shapePool->GetIndex(entity);

				if (shapeComponent.isInstanced && shapeComponent.toRender)
					shapeComponent.shape->AddInstanceID(glm::uvec4(entity, transformIndex, materialIndex, shapeIndex));
			}
		}
	);

	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;
		geometry->UpdateInstanceSsbo();
	}
}

void InstanceSystem::ModelInstances(std::shared_ptr<Registry> registry)
{
	auto modelManager = ModelManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;

		if(model)
			model->ClearInstances();
	}

	if (!transformPool || !modelPool)
		return;

	std::for_each(std::execution::seq, modelPool->GetDenseEntitiesArray().begin(), modelPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) &&
				modelPool->GetComponent(entity).model != nullptr)
			{
				auto& modelComponent = modelPool->GetComponent(entity);
				auto transformIndex = transformPool->GetIndex(entity);
				auto modelIndex = modelPool->GetIndex(entity);

				if (modelComponent.isInstanced && modelComponent.toRender)
					modelComponent.model->AddInstanceID(glm::uvec4(entity, transformIndex, 0, modelIndex));
			}
		}
	);

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;

		if(model)
			model->UpdateInstanceSsbo();
	}
}