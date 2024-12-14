#include "BoxColliderSystem.h"
#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <physx/PxPhysicsAPI.h>

#include "Model/Model.h"
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/Object/ShapeComponent.h"
#include "Registry/Component/Object/ModelComponent.h"
#include "Registry/Component/Physics/BoxColliderComponent.h"
#include "Registry/Component/TransformComponent.h"
#include "Render/OpenGL/ShaderStorageBufferGL.h"
#include "Render/Geometry/Geometry.h"
#include "Render/WireframeRenderer.h"
#include "Settings/GlobalSettings.h"

void BoxColliderSystem::OnStart(std::shared_ptr<Registry> registry)
{

}

void BoxColliderSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto boxColliderPool = registry->GetComponentPool<BoxColliderComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();

	if (!transformPool || !boxColliderPool)
		return;

	auto bcTransformSsbo = resourceManager->GetSsbo("BoxColliderTransform");
	if (bcTransformSsbo->GetBufferHandler() == nullptr)
		bcTransformSsbo->MapBufferRange();
	glm::mat4* bcTransformSsboHandler = static_cast<glm::mat4*>(bcTransformSsbo->GetBufferHandler());

	if (!bcTransformSsboHandler || boxColliderPool->GetSize() > resourceManager->GetComponentSsboSize<BoxColliderComponent>())
		return;

	std::for_each(std::execution::seq, boxColliderPool->GetDenseEntitiesArray().begin(), boxColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) && (boxColliderPool->IsFlagSet(entity, UPDATE_FLAG) || transformPool->IsFlagSet(entity, TC_SCALECHANGED_FLAG)))
			{
				auto& boxCollider = boxColliderPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				auto index = boxColliderPool->GetIndex(entity);

				//Calculate the box geometries automatic from the generated OBB.
				bool hasShape = shapePool && shapePool->HasComponent(entity) && shapePool->GetComponent(entity).shape;
				bool hasModel = modelPool && modelPool->HasComponent(entity) && modelPool->GetComponent(entity).model;
				if (boxCollider.calculateAutomatic && (hasShape || hasModel))
				{
					if (hasShape)
					{
						auto shapeComponent = shapePool->GetComponent(entity);
						boxCollider.halfExtents = shapeComponent.shape->GetObbExtents();
						boxCollider.origin = shapeComponent.shape->GetObbOrigin();
					}
					else
					{
						auto modelComponent = modelPool->GetComponent(entity);
						boxCollider.halfExtents = modelComponent.model->GetObbExtents();
						boxCollider.origin = modelComponent.model->GetObbOrigin();
					}

				}

				//We have to regenerate when transform scale changes!!
				//Obb transform -> get abb from it -> defaultCollider?

				if (boxCollider.bindToTransformScale)
					boxCollider.transformedExtents = boxCollider.halfExtents * glm::abs(transformComponent.scale);
				else
					boxCollider.transformedExtents = boxCollider.halfExtents;

				boxCollider.boxGeometry = PxBoxGeometry(boxCollider.transformedExtents.x, boxCollider.transformedExtents.y, boxCollider.transformedExtents.z);
				boxColliderPool->ResFlag(entity, UPDATE_FLAG);
				boxColliderPool->SetFlag(entity, CHANGED_FLAG);
			}

			//Physics system ussed transformedOrigin, which needs to be updated every frame -> Thats why true || 
			if (true || transformPool->HasComponent(entity) && (boxColliderPool->IsFlagSet(entity, CHANGED_FLAG) || transformPool->IsFlagSet(entity, CHANGED_FLAG) || transformPool->IsFlagSet(entity, TC_SCALECHANGED_FLAG)))
			{
				auto& boxCollider = boxColliderPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				auto index = boxColliderPool->GetIndex(entity);

				if(boxCollider.bindToTransformTranslate)
					boxCollider.transformedOrigin = transformComponent.fullTransform * glm::vec4(boxCollider.origin, 1);
				else
					boxCollider.transformedOrigin = glm::vec4(boxCollider.origin, 1);

				if (!GlobalSettings::GameViewActive && WireframeRenderer::ShowBoxCollider)
				{
					glm::mat4 bcTransform = glm::translate(boxCollider.transformedOrigin) * transformComponent.rotateMatrix * glm::scale(boxCollider.transformedExtents + glm::vec3(0.01));
					bcTransformSsboHandler[index] = bcTransform;
				}
			}
		}
	);

	//bcTransformSsbo->UnMapBuffer();
}

void BoxColliderSystem::OnEnd(std::shared_ptr<Registry> registry)
{
	auto boxColliderPool = registry->GetComponentPool<BoxColliderComponent>();

	if (!boxColliderPool)
		return;

	std::for_each(std::execution::seq, boxColliderPool->GetDenseEntitiesArray().begin(), boxColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			boxColliderPool->ResFlag(entity, CHANGED_FLAG);
		}
	);
}

nlohmann::json BoxColliderSystem::Serialize(Registry* registry, Entity entity)
{
	auto& boxColliderComponent = registry->GetComponent<BoxColliderComponent>(entity);

	nlohmann::json data;
	data["isTrigger"] = boxColliderComponent.isTrigger;
	data["calculateAutomatic"] = boxColliderComponent.calculateAutomatic;
	data["halfExtents"]["x"] = boxColliderComponent.halfExtents.x;
	data["halfExtents"]["y"] = boxColliderComponent.halfExtents.y;
	data["halfExtents"]["z"] = boxColliderComponent.halfExtents.z;
	data["origin"]["x"] = boxColliderComponent.origin.x;
	data["origin"]["y"] = boxColliderComponent.origin.y;
	data["origin"]["z"] = boxColliderComponent.origin.z;
	return data;
}

void BoxColliderSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& boxColliderComponent = registry->GetComponent<BoxColliderComponent>(entity);

	if (data.find("isTrigger") != data.end())
		boxColliderComponent.isTrigger = data["isTrigger"];

	boxColliderComponent.calculateAutomatic = data["calculateAutomatic"];
	boxColliderComponent.halfExtents.x = data["halfExtents"]["x"];
	boxColliderComponent.halfExtents.y = data["halfExtents"]["y"];
	boxColliderComponent.halfExtents.z = data["halfExtents"]["z"];
	boxColliderComponent.origin.x = data["origin"]["x"];
	boxColliderComponent.origin.y = data["origin"]["y"];
	boxColliderComponent.origin.z = data["origin"]["z"];

	registry->SetFlag<BoxColliderComponent>(entity, UPDATE_FLAG);
}