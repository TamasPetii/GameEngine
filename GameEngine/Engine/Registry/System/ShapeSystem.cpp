#include "ShapeSystem.h"

void ShapeSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void ShapeSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	if (!shapePool)
		return;

	std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (!registry->HasComponent<DefaultCollider>(entity))
			{
				registry->AddComponent<DefaultCollider>(entity);
			}
		}
	);
}

void ShapeSystem::OnEnd(std::shared_ptr<Registry> registry)
{
}

nlohmann::json ShapeSystem::Serialize(Registry* registry, Entity entity)
{
	auto& shapeComponent = registry->GetComponent<ShapeComponent>(entity);

	nlohmann::json data;
	data["toRender"] = shapeComponent.toRender;
	data["castShadow"] = shapeComponent.castShadow;
	data["isInstanced"] = shapeComponent.isInstanced;
	data["receiveShadow"] = shapeComponent.receiveShadow;
	data["shape"] = shapeComponent.shape ? shapeComponent.shape->GetName() : "none";

	return data;
}

void ShapeSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& shapeComponent = registry->GetComponent<ShapeComponent>(entity);
	auto resourceManager = ResourceManager::Instance();

	shapeComponent.toRender = data["toRender"];
	shapeComponent.castShadow = data["castShadow"];
	shapeComponent.isInstanced = data["isInstanced"];
	shapeComponent.receiveShadow = data["receiveShadow"];
	shapeComponent.shape = data["shape"] == "none" ? nullptr : resourceManager->GetGeometry(data["shape"]);

	registry->SetFlag<ShapeComponent>(entity, UPDATE_FLAG);
}