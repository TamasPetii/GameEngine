#include "BoxColliderSystem.h"

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

	std::for_each(std::execution::seq, boxColliderPool->GetDenseEntitiesArray().begin(), boxColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (boxColliderPool->IsFlagSet(entity, UPDATE_FLAG) && transformPool->HasComponent(entity))
			{
				auto& boxCollider = boxColliderPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);

				//Calculate the box geometries automatic from the generated OBB.
				bool hasShape = shapePool && shapePool->HasComponent(entity) && shapePool->GetComponent(entity).shape;
				bool hasModel = modelPool && modelPool->HasComponent(entity) && modelPool->GetComponent(entity).model;
				if (boxCollider.calculateAutomatic && (hasShape || hasModel))
				{
					if (hasShape)
					{
						auto shapeComponent = shapePool->GetComponent(entity);
						boxCollider.halfExtents = shapeComponent.shape->GetObbExtents();
					}
					else
					{
						auto modelComponent = modelPool->GetComponent(entity);
						boxCollider.halfExtents = modelComponent.model->GetObbExtents();
					}
				}

				//We have to regenerate when transform scale changes!!
				boxCollider.boxGeometry = PxBoxGeometry(boxCollider.halfExtents.x, boxCollider.halfExtents.y, boxCollider.halfExtents.z);
				boxColliderPool->ResFlag(entity, UPDATE_FLAG);
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