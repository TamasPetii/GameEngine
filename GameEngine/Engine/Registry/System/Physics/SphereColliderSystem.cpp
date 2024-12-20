#include "SphereColliderSystem.h"

#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model/Model.h"
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/Object/ShapeComponent.h"
#include "Registry/Component/Object/ModelComponent.h"
#include "Registry/Component/Physics/SphereColliderComponent.h"
#include "Registry/Component/TransformComponent.h"
#include "Render/OpenGL/ShaderStorageBufferGL.h"
#include "Render/Geometry/Geometry.h"
#include "Settings/GlobalSettings.h"
#include "Render/WireframeRenderer.h"

void SphereColliderSystem::OnStart(std::shared_ptr<Registry> registry)
{

}

void SphereColliderSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto sphereColliderPool = registry->GetComponentPool<SphereColliderComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();

	if (!transformPool || !sphereColliderPool)
		return;

	auto scTransformSsbo = resourceManager->GetSsbo("SphereColliderTransform");
	if (scTransformSsbo->GetBufferHandler() == nullptr)
		scTransformSsbo->MapBufferRange();
	glm::mat4* scTransformSsboHandler = static_cast<glm::mat4*>(scTransformSsbo->GetBufferHandler());

	if (!scTransformSsboHandler || sphereColliderPool->GetSize() > resourceManager->GetComponentSsboSize<SphereColliderComponent>())
		return;

	std::for_each(std::execution::par, sphereColliderPool->GetDenseEntitiesArray().begin(), sphereColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			//Need to determine if the transform scale changed
			if (transformPool->HasComponent(entity) && (sphereColliderPool->IsFlagSet(entity, UPDATE_FLAG) || transformPool->IsFlagSet(entity, CUSTOM1_FLAG)))
			{
				auto& sphereCollider = sphereColliderPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				auto index = sphereColliderPool->GetIndex(entity);

				//Calculate the box geometries automatic from the generated OBB.
				bool hasShape = shapePool && shapePool->HasComponent(entity) && shapePool->GetComponent(entity).shape;
				bool hasModel = modelPool && modelPool->HasComponent(entity) && modelPool->GetComponent(entity).model;

				if (sphereCollider.calculateAutomatic && (hasShape || hasModel))
				{
					if (hasShape)
					{
						auto shapeComponent = shapePool->GetComponent(entity);
						auto& extents = shapeComponent.shape->GetObbExtents();
						float maxExtent = glm::max(glm::max(extents.x, extents.y), extents.z);
						sphereCollider.radius = maxExtent;
						sphereCollider.origin = shapeComponent.shape->GetObbOrigin();
					}
					else
					{
						auto modelComponent = modelPool->GetComponent(entity);
						auto& extents = modelComponent.model->GetObbExtents();
						float maxExtent = glm::max(glm::max(extents.x, extents.y), extents.z);
						sphereCollider.radius = maxExtent;
						sphereCollider.origin = modelComponent.model->GetObbOrigin();
					}
				}

				//We have to regenerate when transform scale changes!!
				//float maxScale = glm::max(glm::max(transformComponent.scale.x, transformComponent.scale.y), transformComponent.scale.z);
				
				if (sphereCollider.bindToTransformTranslate)
				{
					glm::vec3 absScale = glm::abs(transformComponent.scale);
					float maxScale = glm::max(glm::max(absScale.x, absScale.y), absScale.z);
					sphereCollider.transformedRadius = sphereCollider.radius * maxScale;
				}
				else
				{
					sphereCollider.transformedRadius = sphereCollider.radius;
				}

				sphereCollider.transformedOrigin = glm::vec4(sphereCollider.origin, 1);
				sphereCollider.sphereGeometry = PxSphereGeometry(sphereCollider.transformedRadius);
				sphereColliderPool->ResFlag(entity, UPDATE_FLAG);
				sphereColliderPool->SetFlag(entity, CHANGED_FLAG);
			}

			if (transformPool->HasComponent(entity) && (sphereColliderPool->IsFlagSet(entity, CHANGED_FLAG) || sphereColliderPool->IsFlagSet(entity, CUSTOM1_FLAG) || transformPool->IsFlagSet(entity, CHANGED_FLAG) || transformPool->IsFlagSet(entity, CUSTOM1_FLAG)))
			{
				auto& sphereCollider = sphereColliderPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				
				if(sphereCollider.bindToTransformTranslate)
					sphereCollider.transformedOrigin = transformComponent.fullTransform * glm::vec4(sphereCollider.origin, 1);
				else
					sphereCollider.transformedOrigin = glm::vec4(sphereCollider.origin, 1);

				if (!GlobalSettings::GameViewActive)
				{
					auto index = sphereColliderPool->GetIndex(entity);
					glm::mat4 scTransform = glm::translate(sphereCollider.transformedOrigin) * transformComponent.rotateMatrix * glm::scale(glm::vec3(sphereCollider.transformedRadius + 0.035));
					scTransformSsboHandler[index] = scTransform;
				}

				sphereColliderPool->ResFlag(entity, CUSTOM1_FLAG);
			}
		}
	);

	//scTransformSsbo->UnMapBuffer();
}

void SphereColliderSystem::OnEnd(std::shared_ptr<Registry> registry)
{
	auto sphereColliderPool = registry->GetComponentPool<SphereColliderComponent>();

	if (!sphereColliderPool)
		return;

	std::for_each(std::execution::seq, sphereColliderPool->GetDenseEntitiesArray().begin(), sphereColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			sphereColliderPool->ResFlag(entity, CHANGED_FLAG);
		}
	);
}

nlohmann::json SphereColliderSystem::Serialize(Registry* registry, Entity entity)
{
	auto& sphereColliderComponent = registry->GetComponent<SphereColliderComponent>(entity);

	nlohmann::json data;
	data["isTrigger"] = sphereColliderComponent.isTrigger;
	data["calculateAutomatic"] = sphereColliderComponent.calculateAutomatic;
	data["radius"] = sphereColliderComponent.radius;
	data["origin"]["x"] = sphereColliderComponent.origin.x;
	data["origin"]["y"] = sphereColliderComponent.origin.y;
	data["origin"]["z"] = sphereColliderComponent.origin.z;
	return data;
}

void SphereColliderSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& sphereColliderComponent = registry->GetComponent<SphereColliderComponent>(entity);

	if (data.find("isTrigger") != data.end())
		sphereColliderComponent.isTrigger = data["isTrigger"];

	sphereColliderComponent.calculateAutomatic = data["calculateAutomatic"];
	sphereColliderComponent.radius = data["radius"];
	sphereColliderComponent.origin.x = data["origin"]["x"];
	sphereColliderComponent.origin.y = data["origin"]["y"];
	sphereColliderComponent.origin.z = data["origin"]["z"];
	registry->SetFlag<SphereColliderComponent>(entity, UPDATE_FLAG);
}