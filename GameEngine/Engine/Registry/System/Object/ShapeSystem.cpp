#include "ShapeSystem.h"

#include <algorithm>
#include <execution>

#include "Registry/Registry.h"
#include "Render/Geometry/Geometry.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/Object/ShapeComponent.h"
#include "Registry/Component/Physics/DefaultCollider.h"
#include "Render/OpenGL/ShaderStorageBufferGL.h"

void ShapeSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void ShapeSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();
	auto defaultColliderPool = registry->GetComponentPool<DefaultCollider>();

	if (!shapePool)
		return;

	auto shDataSsbo = resourceManager->GetSsbo("ShapeData");
	if (shDataSsbo->GetBufferHandler() == nullptr)
		shDataSsbo->MapBufferRange();
	ShapeGLSL* shDataSsboHandler = static_cast<ShapeGLSL*>(shDataSsbo->GetBufferHandler());

	if (!shDataSsboHandler || shapePool->GetSize() > resourceManager->GetComponentSsboSize<ShapeComponent>())
		return;

	std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (!defaultColliderPool)
				registry->AddComponent<DefaultCollider>(entity);
			else if (defaultColliderPool && !defaultColliderPool->HasComponent(entity))
				defaultColliderPool->AddComponent(entity);

			if (shapePool->IsFlagSet(entity, UPDATE_FLAG) && shapePool->GetComponent(entity).shape)
			{
				auto& shapeComponent = shapePool->GetComponent(entity);
				auto index = shapePool->GetIndex(entity);
				 
				shDataSsboHandler[index] = ShapeGLSL(shapeComponent);
				shapePool->ResFlag(entity, UPDATE_FLAG);
				shapePool->SetFlag(entity, CHANGED_FLAG);
			}
		}
	);

	//shDataSsbo->UnMapBuffer();
}

void ShapeSystem::OnEnd(std::shared_ptr<Registry> registry)
{
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	if (!shapePool)
		return;

	std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			shapePool->ResFlag(entity, CHANGED_FLAG);
		}
	);
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