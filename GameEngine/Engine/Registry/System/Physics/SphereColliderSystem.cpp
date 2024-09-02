#include "SphereColliderSystem.h"

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

	static glm::mat4* scTransformSsboHandler = nullptr;

	if (!scTransformSsboHandler)
	{
		auto scTransformSsbo = resourceManager->GetSsbo("SphereColliderTransform");
		scTransformSsboHandler = static_cast<glm::mat4*>(scTransformSsbo->MapBufferRange());
	}

	std::for_each(std::execution::seq, sphereColliderPool->GetDenseEntitiesArray().begin(), sphereColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			//Need to determine if the transform scale changed
			if (sphereColliderPool->IsFlagSet(entity, UPDATE_FLAG) && transformPool->HasComponent(entity))
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

					glm::vec3 absScale = glm::abs(transformComponent.scale);
					float maxScale = glm::max(glm::max(absScale.x, absScale.y), absScale.z);
					sphereCollider.radius *= maxScale;
					sphereCollider.origin = transformComponent.fullTransform* glm::vec4(sphereCollider.origin, 1);
				}

				//We have to regenerate when transform scale changes!!
				//float maxScale = glm::max(glm::max(transformComponent.scale.x, transformComponent.scale.y), transformComponent.scale.z);
				sphereCollider.sphereGeometry = PxSphereGeometry(sphereCollider.radius);

				glm::mat4 scTransform = glm::translate(sphereCollider.origin) * glm::scale(glm::vec3(sphereCollider.radius + 0.035));
				scTransformSsboHandler[index] = scTransform;

				sphereColliderPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);

	//scTransformSsbo->UnMapBuffer();
}

nlohmann::json SphereColliderSystem::Serialize(Registry* registry, Entity entity)
{
	auto& sphereColliderComponent = registry->GetComponent<SphereColliderComponent>(entity);

	nlohmann::json data;
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
	sphereColliderComponent.calculateAutomatic = data["calculateAutomatic"];
	sphereColliderComponent.radius = data["radius"];
	sphereColliderComponent.origin.x = data["origin"]["x"];
	sphereColliderComponent.origin.y = data["origin"]["y"];
	sphereColliderComponent.origin.z = data["origin"]["z"];
	registry->SetFlag<SphereColliderComponent>(entity, UPDATE_FLAG);
}