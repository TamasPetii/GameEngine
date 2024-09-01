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

	std::for_each(std::execution::seq, sphereColliderPool->GetDenseEntitiesArray().begin(), sphereColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			//Need to determine if the transform scale changed
			if (sphereColliderPool->IsFlagSet(entity, UPDATE_FLAG) && transformPool->HasComponent(entity))
			{
				auto& sphereCollider = sphereColliderPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);

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
					}
					else
					{
						auto modelComponent = modelPool->GetComponent(entity);
						auto& extents = modelComponent.model->GetObbExtents();
						float maxExtent = glm::max(glm::max(extents.x, extents.y), extents.z);
						sphereCollider.radius = maxExtent;
					}

					glm::vec3 absScale = glm::abs(transformComponent.scale);
					float maxScale = glm::max(glm::max(absScale.x, absScale.y), absScale.z);
					sphereCollider.radius *= maxScale;
				}

				//We have to regenerate when transform scale changes!!
				//float maxScale = glm::max(glm::max(transformComponent.scale.x, transformComponent.scale.y), transformComponent.scale.z);
				sphereCollider.sphereGeometry = PxSphereGeometry(sphereCollider.radius);
				sphereColliderPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);
}

nlohmann::json SphereColliderSystem::Serialize(Registry* registry, Entity entity)
{
	//auto& meshCollider = registry->GetComponent<SphereColliderComponent>(entity);

	nlohmann::json data;
	return data;
}

void SphereColliderSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	//auto& meshCollider = registry->GetComponent<SphereCollider>(entity);

	//registry->SetFlag<SphereColliderComponent>(entity, REGENERATE_FLAG);
	//registry->SetFlag<SphereColliderComponent>(entity, UPDATE_FLAG);
}