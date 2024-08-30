#include "BoxColliderSystem.h"

void BoxColliderSystem::OnStart(std::shared_ptr<Registry> registry)
{

}

void BoxColliderSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto boxColliderPool = registry->GetComponentPool<BoxColliderComponent>();

	if (!transformPool || !boxColliderPool)
		return;

	std::for_each(std::execution::seq, boxColliderPool->GetDenseEntitiesArray().begin(), boxColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			//(transformPool->HasComponent(entity) && transformPool->IsFlagSet(entity, CHANGED_FLAG))
			if (true || boxColliderPool->IsFlagSet(entity, UPDATE_FLAG))
			{
				auto& boxCollider = boxColliderPool->GetComponent(entity);

				//Calculate the box geometries automatic from the generated OBB.
				if (boxCollider.calculateAutomatic)
				{
					auto defaultColliderPool = registry->GetComponentPool<DefaultCollider>();
					if (defaultColliderPool && defaultColliderPool->HasComponent(entity))
					{
						auto defaultCollider = defaultColliderPool->GetComponent(entity);
						boxCollider.halfExtents = (defaultCollider.aabbMax - defaultCollider.aabbMin) / 2.f;
					}
				}

				//We have to regenerate when transform scale changes!!
				boxCollider.boxGeometry = PxBoxGeometry(boxCollider.halfExtents.x, boxCollider.halfExtents.y, boxCollider.halfExtents.z);
			}
		}
	);
}

nlohmann::json BoxColliderSystem::Serialize(Registry* registry, Entity entity)
{
	//auto& meshCollider = registry->GetComponent<BoxColliderComponent>(entity);

	nlohmann::json data;
	return data;
}

void BoxColliderSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	//auto& meshCollider = registry->GetComponent<BoxColliderComponent>(entity);

	//registry->SetFlag<BoxColliderComponent>(entity, REGENERATE_FLAG);
	//registry->SetFlag<BoxColliderComponent>(entity, UPDATE_FLAG);
}