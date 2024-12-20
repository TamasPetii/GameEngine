#include "TransformSystem.h"

#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/TransformComponent.h"
#include "Render/OpenGL/ShaderStorageBufferGL.h"

void TransformSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void TransformSystem::OnUpdate(std::shared_ptr<Registry> registry)
{	
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();

	if (!transformPool)
		return;

	auto tsDataSsbo = resourceManager->GetSsbo("TransformData");
	if (tsDataSsbo->GetBufferHandler() == nullptr)
		tsDataSsbo->MapBufferRange();
	TransformGLSL* tsDataSsboHandler = static_cast<TransformGLSL*>(tsDataSsbo->GetBufferHandler());

	if (!tsDataSsboHandler || transformPool->GetSize() > resourceManager->GetComponentSsboSize<TransformComponent>())
		return;

	std::for_each(std::execution::par, transformPool->GetDenseEntitiesArray().begin(), transformPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->IsFlagSet(entity, UPDATE_FLAG))
			{
				auto& transformComponent = transformPool->GetComponent(entity);
				auto index = transformPool->GetIndex(entity);

				transformComponent.translateMatrix = glm::translate(transformComponent.translate);
				transformComponent.rotateMatrix = glm::rotate(glm::radians(transformComponent.rotate.z), glm::vec3(0, 0, 1))
												* glm::rotate(glm::radians(transformComponent.rotate.y), glm::vec3(0, 1, 0))
												* glm::rotate(glm::radians(transformComponent.rotate.x), glm::vec3(1, 0, 0));
				transformComponent.scaleMatrix = glm::scale(transformComponent.scale);

				transformComponent.modelTransform = transformComponent.translateMatrix * transformComponent.rotateMatrix * transformComponent.scaleMatrix;
				transformComponent.modelTransformIT = glm::inverse(glm::transpose(transformComponent.modelTransform));

				transformComponent.fullTransform = transformComponent.modelTransform;
				transformComponent.fullTransformIT = transformComponent.modelTransformIT;

				tsDataSsboHandler[index] = TransformGLSL(transformComponent);
				transformPool->ResFlag(entity, UPDATE_FLAG);
				transformPool->SetFlag(entity, CHANGED_FLAG);
			}
		}
	);

	//tsDataSsbo->UnMapBuffer();
}

void TransformSystem::OnEnd(std::shared_ptr<Registry> registry)
{
	auto transformPool = registry->GetComponentPool<TransformComponent>();

	if (!transformPool)
		return;

	std::for_each(std::execution::seq, transformPool->GetDenseEntitiesArray().begin(), transformPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			transformPool->ResFlag(entity, CHANGED_FLAG);
			transformPool->ResFlag(entity, CUSTOM1_FLAG);
		}
	);
}

nlohmann::json TransformSystem::Serialize(Registry* registry, Entity entity)
{
	auto& transformComponent = registry->GetComponent<TransformComponent>(entity);

	nlohmann::json data;
	data["translate"]["x"] = transformComponent.translate.x;
	data["translate"]["y"] = transformComponent.translate.y;
	data["translate"]["z"] = transformComponent.translate.z;
	data["rotate"]["x"] = transformComponent.rotate.x;
	data["rotate"]["y"] = transformComponent.rotate.y;
	data["rotate"]["z"] = transformComponent.rotate.z;
	data["scale"]["x"] = transformComponent.scale.x;
	data["scale"]["y"] = transformComponent.scale.y;
	data["scale"]["z"] = transformComponent.scale.z;

	return data;
}

void TransformSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& transformComponent = registry->GetComponent<TransformComponent>(entity);
	transformComponent.translate.x = data["translate"]["x"];
	transformComponent.translate.y = data["translate"]["y"];
	transformComponent.translate.z = data["translate"]["z"];
	transformComponent.rotate.x = data["rotate"]["x"];
	transformComponent.rotate.y = data["rotate"]["y"];
	transformComponent.rotate.z = data["rotate"]["z"];
	transformComponent.scale.x = data["scale"]["x"];
	transformComponent.scale.y = data["scale"]["y"];
	transformComponent.scale.z = data["scale"]["z"];
	registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
}