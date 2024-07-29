#include "ModelSystem.h"
#include "Manager/ModelManager.h"

void ModelSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void ModelSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto modelPool = registry->GetComponentPool<ModelComponent>();

	if (!modelPool)
		return;

	std::for_each(std::execution::seq, modelPool->GetDenseEntitiesArray().begin(), modelPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (!registry->HasComponent<DefaultCollider>(entity))
			{
				registry->AddComponent<DefaultCollider>(entity);
			}
		}
	);
}

void ModelSystem::OnEnd(std::shared_ptr<Registry> registry)
{
}

nlohmann::json ModelSystem::Serialize(Registry* registry, Entity entity)
{
	auto& modelComponent = registry->GetComponent<ModelComponent>(entity);

	nlohmann::json data;
	data["toRender"] = modelComponent.toRender;
	data["castShadow"] = modelComponent.castShadow;
	data["isInstanced"] = modelComponent.isInstanced;
	data["receiveShadow"] = modelComponent.receiveShadow;
	data["model"] = modelComponent.model ? modelComponent.model->GetPath() : "none";

	return data;
}

void ModelSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& modelComponent = registry->GetComponent<ModelComponent>(entity);
	auto modelManager = ModelManager::Instance();

	modelComponent.toRender = data["toRender"];
	modelComponent.castShadow = data["castShadow"];
	modelComponent.isInstanced = data["isInstanced"];
	modelComponent.receiveShadow = data["receiveShadow"];
	modelComponent.model = data["model"] == "none" ? nullptr : modelManager->LoadModel(data["model"]);

	registry->SetFlag<ModelComponent>(entity, UPDATE_FLAG);
}