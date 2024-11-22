#include "MaterialSystem.h"
#include "Manager/TextureManager.h"

void MaterialSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void MaterialSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto materialPool = registry->GetComponentPool<MaterialComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();

	if (!materialPool)
		return;

	auto mtDataSsbo = resourceManager->GetSsbo("MaterialData");
	if (mtDataSsbo->GetBufferHandler() == nullptr)
		mtDataSsbo->MapBufferRange();
	MaterialGLSL* mtDataSsboHandler = static_cast<MaterialGLSL*>(mtDataSsbo->GetBufferHandler());
	
	if (!mtDataSsboHandler || materialPool->GetSize() > resourceManager->GetComponentSsboSize<MaterialComponent>())
		return;

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
	data["color"]["x"] = materialComponent.color.x;
	data["color"]["y"] = materialComponent.color.y;
	data["color"]["z"] = materialComponent.color.z;
	data["color"]["w"] = materialComponent.color.w;
	data["shinniness"] = materialComponent.shinniness;
	data["textureScale"]["x"] = materialComponent.textureScale.x;
	data["textureScale"]["y"] = materialComponent.textureScale.y;

	std::string diffusePath = "none";
	if (materialComponent.diffuse)
	{
		if (materialComponent.diffuse->GetPath().substr(0, GlobalSettings::ProjectPath.size()) == GlobalSettings::ProjectPath)
			diffusePath = materialComponent.diffuse->GetPath().substr(GlobalSettings::ProjectPath.size() + 1); //Te +1 for deleting / from the start of the path
		else
			diffusePath = materialComponent.diffuse->GetPath();
	}

	data["diffuseTexture"] = diffusePath;

	std::string normalPath = "none";
	if (materialComponent.normal)
	{
		if (materialComponent.normal->GetPath().substr(0, GlobalSettings::ProjectPath.size()) == GlobalSettings::ProjectPath)
			normalPath = materialComponent.normal->GetPath().substr(GlobalSettings::ProjectPath.size() + 1); //Te +1 for deleting / from the start of the path
		else
			normalPath = materialComponent.normal->GetPath();
	}
	data["normalTexture"] = normalPath;

	std::string specularPath = "none";
	if (materialComponent.specular)
	{
		if (materialComponent.specular->GetPath().substr(0, GlobalSettings::ProjectPath.size()) == GlobalSettings::ProjectPath)
			specularPath = materialComponent.specular->GetPath().substr(GlobalSettings::ProjectPath.size() + 1); //Te +1 for deleting / from the start of the path
		else
			specularPath = materialComponent.specular->GetPath();
	}
	data["specularTexture"] = specularPath;

	return data;
}

void MaterialSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& materialComponent = registry->GetComponent<MaterialComponent>(entity);
	auto textureManager = TextureManager::Instance();
	
	materialComponent.useBloom = static_cast<bool>(data["useBloom"]);
	materialComponent.color = glm::vec4(data["color"]["x"], data["color"]["y"], data["color"]["z"], data["color"]["w"]);
	materialComponent.shinniness = data["shinniness"];
	materialComponent.textureScale = glm::vec2(data["textureScale"]["x"], data["textureScale"]["y"]);
	materialComponent.diffuse = data["diffuseTexture"] == "none" ? nullptr : textureManager->LoadImageTexture(static_cast<std::string>(data["diffuseTexture"]));
	materialComponent.normal = data["normalTexture"] == "none" ? nullptr : textureManager->LoadImageTexture(static_cast<std::string>(data["normalTexture"]));
	materialComponent.specular = data["specularTexture"] == "none" ? nullptr : textureManager->LoadImageTexture(static_cast<std::string>(data["specularTexture"]));

	registry->SetFlag<MaterialComponent>(entity, UPDATE_FLAG);
}