#include "SphereColliderSystem.h"

void SphereColliderSystem::OnStart(std::shared_ptr<Registry> registry)
{

}

void SphereColliderSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto sphereColliderPool = registry->GetComponentPool<SphereColliderComponent>();

	if (!transformPool || !sphereColliderPool)
		return;

	std::for_each(std::execution::seq, sphereColliderPool->GetDenseEntitiesArray().begin(), sphereColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			//Need to determine if the transform scale changed
			if (true || sphereColliderPool->IsFlagSet(entity, UPDATE_FLAG))
			{
				auto& sphereCollider = sphereColliderPool->GetComponent(entity);

				//Calculate the sphere geometries automatic from the generated OBB.
				if (sphereCollider.calculateAutomatic)
				{
					auto defaultColliderPool = registry->GetComponentPool<DefaultCollider>();
					if (defaultColliderPool && defaultColliderPool->HasComponent(entity))
					{
						auto defaultCollider = defaultColliderPool->GetComponent(entity);
						glm::vec3 extents = (defaultCollider.aabbMax - defaultCollider.aabbMin) / 2.f;
						sphereCollider.radius = 2 * glm::max(glm::max(extents.x, extents.y), extents.z);
					}
				}

				//We have to regenerate when transform scale changes!!
				sphereCollider.sphereGeometry = PxSphereGeometry(sphereCollider.radius);
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