#include "ModelSystem.h"

#include <algorithm>
#include <execution>

#include "Model/Model.h"
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Manager/ModelManager.h"
#include "Registry/Component/Object/ModelComponent.h"
#include "Registry/Component/Physics/DefaultCollider.h"

#include "Render/OpenGL/ShaderStorageBufferGL.h"
#include "Settings/GlobalSettings.h"

void ModelSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void ModelSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto modelPool = registry->GetComponentPool<ModelComponent>();

	if (!modelPool)
		return;

	auto mdDataSsbo = resourceManager->GetSsbo("ModelData");
	if (mdDataSsbo->GetBufferHandler() == nullptr)
		mdDataSsbo->MapBufferRange();
	ModelGLSL* mdDataSsboHandler = static_cast<ModelGLSL*>(mdDataSsbo->GetBufferHandler());

	if (!mdDataSsboHandler || modelPool->GetSize() > resourceManager->GetComponentSsboSize<ModelComponent>())
		return;

	std::for_each(std::execution::seq, modelPool->GetDenseEntitiesArray().begin(), modelPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (!registry->HasComponent<DefaultCollider>(entity))
			{
				registry->AddComponent<DefaultCollider>(entity);
			}

			if (modelPool->IsFlagSet(entity, UPDATE_FLAG) && modelPool->GetComponent(entity).model)
			{
				auto& modelComponent = modelPool->GetComponent(entity);
				auto index = modelPool->GetIndex(entity);

				mdDataSsboHandler[index] = ModelGLSL(modelComponent);
				modelPool->SetFlag(entity, CHANGED_FLAG);
				modelPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);

	//mdDataSsbo->UnMapBuffer();
}

void ModelSystem::OnEnd(std::shared_ptr<Registry> registry)
{
	auto modelPool = registry->GetComponentPool<ModelComponent>();

	if (!modelPool)
		return;

	std::for_each(std::execution::seq, modelPool->GetDenseEntitiesArray().begin(), modelPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			modelPool->ResFlag(entity, CHANGED_FLAG);
		}
	);
}

nlohmann::json ModelSystem::Serialize(Registry* registry, Entity entity)
{
	auto& modelComponent = registry->GetComponent<ModelComponent>(entity);

	nlohmann::json data;
	data["toRender"] = modelComponent.toRender;
	data["castShadow"] = modelComponent.castShadow;
	data["isInstanced"] = modelComponent.isInstanced;
	data["receiveShadow"] = modelComponent.receiveShadow;

	std::string modelPath = "none";

	if (modelComponent.model)
	{
		if (modelComponent.model->GetPath().substr(0, GlobalSettings::ProjectPath.size()) == GlobalSettings::ProjectPath)
			modelPath = modelComponent.model->GetPath().substr(GlobalSettings::ProjectPath.size() + 1); //The +1 for deleting / from the start of the path
		else
			modelPath = modelComponent.model->GetPath();
	}
		
	data["model"] = modelPath;

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