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

	static glm::mat4* bcTransformSsboHandler = nullptr;

	if (!bcTransformSsboHandler)
	{
		auto bcTransformSsbo = resourceManager->GetSsbo("BoxColliderTransform");
		bcTransformSsboHandler = static_cast<glm::mat4*>(bcTransformSsbo->MapBufferRange());
	}

	std::for_each(std::execution::seq, boxColliderPool->GetDenseEntitiesArray().begin(), boxColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			auto& boxCollider = boxColliderPool->GetComponent(entity);
			auto index = boxColliderPool->GetIndex(entity);

			if (boxColliderPool->IsFlagSet(entity, UPDATE_FLAG) && transformPool->HasComponent(entity))
			{
				auto& transformComponent = transformPool->GetComponent(entity);

				//Calculate the box geometries automatic from the generated OBB.
				bool hasShape = shapePool && shapePool->HasComponent(entity) && shapePool->GetComponent(entity).shape;
				bool hasModel = modelPool && modelPool->HasComponent(entity) && modelPool->GetComponent(entity).model;
				if (boxCollider.calculateAutomatic)
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

					boxCollider.halfExtents *= glm::abs(transformComponent.scale);
				}

				//We have to regenerate when transform scale changes!!
				boxCollider.boxGeometry = PxBoxGeometry(boxCollider.halfExtents.x, boxCollider.halfExtents.y, boxCollider.halfExtents.z);		
				boxColliderPool->ResFlag(entity, UPDATE_FLAG);
			}

			if (false && transformPool->HasComponent(entity))
			{
				auto& transformComponent = transformPool->GetComponent(entity);
				glm::vec3 origin = transformComponent.fullTransform * glm::vec4(boxCollider.origin, 1);
				glm::mat4 bcTransform = glm::translate(origin) * glm::scale(boxCollider.halfExtents + glm::vec3(0.01));
				bcTransformSsboHandler[index] = bcTransform;
			}
		}
	);

	//bcTransformSsbo->UnMapBuffer();
}

nlohmann::json BoxColliderSystem::Serialize(Registry* registry, Entity entity)
{
	auto& boxColliderComponent = registry->GetComponent<BoxColliderComponent>(entity);

	nlohmann::json data;
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

	boxColliderComponent.calculateAutomatic = data["calculateAutomatic"];
	boxColliderComponent.halfExtents.x = data["halfExtents"]["x"];
	boxColliderComponent.halfExtents.y = data["halfExtents"]["y"];
	boxColliderComponent.halfExtents.z = data["halfExtents"]["z"];
	boxColliderComponent.origin.x = data["origin"]["x"];
	boxColliderComponent.origin.y = data["origin"]["y"];
	boxColliderComponent.origin.z = data["origin"]["z"];

	registry->SetFlag<BoxColliderComponent>(entity, UPDATE_FLAG);
}