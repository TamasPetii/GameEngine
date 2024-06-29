#include "MaterialSystem.h"
#include "Manager/TextureManager.h"

void MaterialSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void MaterialSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto materialPool = registry->GetComponentPool<MaterialComponent>();

	if (!materialPool)
		return;

	static bool init = true;
	static MaterialGLSL* mtDataSsboHandler = nullptr;
	if (init)
	{
		init = false;
		auto mtDataSsbo = resourceManager->GetSsbo("MaterialData");
		mtDataSsboHandler = static_cast<MaterialGLSL*>(mtDataSsbo->MapBuffer(GL_WRITE_ONLY));
	}

	std::for_each(std::execution::seq, materialPool->GetDenseEntitiesArray().begin(), materialPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (materialPool->IsFlagSet(entity, UPDATE_FLAG))
			{
				auto& materialComponent = materialPool->GetComponent(entity);
				auto index = materialPool->GetIndex(entity);

				mtDataSsboHandler[index] = MaterialGLSL(materialComponent);
				materialPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);

	//mtDataSsbo->UnMapBuffer();
}

nlohmann::json MaterialSystem::Serialize(Registry* registry, Entity entity)
{
	auto& materialComponent = registry->GetComponent<MaterialComponent>(entity);

	nlohmann::json data;
	data["useBloom"] = materialComponent.useBloom;
	data["useEnv"] = materialComponent.useEnv;
	data["color"]["x"] = materialComponent.color.x;
	data["color"]["y"] = materialComponent.color.y;
	data["color"]["z"] = materialComponent.color.z;
	data["color"]["w"] = materialComponent.color.w;
	data["shinniness"] = materialComponent.shinniness;
	data["textureScale"]["x"] = materialComponent.textureScale.x;
	data["textureScale"]["y"] = materialComponent.textureScale.y;
	data["diffuseTexture"] = materialComponent.diffuse ? materialComponent.diffuse->GetPath() : "none";
	data["normalTexture"] = materialComponent.normal ? materialComponent.normal->GetPath() : "none";
	data["specularTexture"] = materialComponent.specular ? materialComponent.specular->GetPath() : "none";

	return data;
}

void MaterialSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& materialComponent = registry->GetComponent<MaterialComponent>(entity);
	auto textureManager = TextureManager::Instance();
	
	materialComponent.useBloom = static_cast<bool>(data["useBloom"]);
	materialComponent.useEnv = static_cast<bool>(data["useEnv"]);
	materialComponent.color = glm::vec4(data["color"]["x"], data["color"]["y"], data["color"]["z"], data["color"]["w"]);
	materialComponent.shinniness = data["shinniness"];
	materialComponent.textureScale = glm::vec2(data["textureScale"]["x"], data["textureScale"]["y"]);
	materialComponent.diffuse = data["diffuseTexture"] == "none" ? nullptr : textureManager->LoadImageTexture(static_cast<std::string>(data["diffuseTexture"]));
	materialComponent.normal = data["normalTexture"] == "none" ? nullptr : textureManager->LoadImageTexture(static_cast<std::string>(data["normalTexture"]));
	materialComponent.specular = data["specularTexture"] == "none" ? nullptr : textureManager->LoadImageTexture(static_cast<std::string>(data["specularTexture"]));

	registry->SetFlag<MaterialComponent>(entity, UPDATE_FLAG);
}