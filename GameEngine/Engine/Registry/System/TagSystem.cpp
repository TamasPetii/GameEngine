#include "TagSystem.h"

void TagSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void TagSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
}

void TagSystem::OnEnd(std::shared_ptr<Registry> registry)
{
}

nlohmann::json TagSystem::Serialize(Registry* registry, Entity entity)
{
	auto& tagComponent = registry->GetComponent<TagComponent>(entity);

	nlohmann::json data;
	data["name"] = tagComponent.name;
	data["tag"] = tagComponent.tag;
	return data;
}

void TagSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& tagComponent = registry->GetComponent<TagComponent>(entity);

	tagComponent.name = data["name"];
	tagComponent.tag = data["tag"];

	registry->SetFlag<TagComponent>(entity, UPDATE_FLAG);
}