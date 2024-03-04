#include "SphereColliderSystem.h"

void SphereColliderSystem::OnStart(std::shared_ptr<Registry> registry)
{

}

void SphereColliderSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto sphereColliderPool = registry->GetComponentPool<SphereCollider>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	static bool init = true;
	static glm::mat4* scTransformSsboHandler = nullptr;
	if (init)
	{
		init = false;
		auto scTransformSsbo = resourceManager->GetSsbo("SphereColliderTransform");
		scTransformSsboHandler = static_cast<glm::mat4*>(scTransformSsbo->MapBuffer(GL_WRITE_ONLY));
	}

	std::for_each(std::execution::par, sphereColliderPool->GetDenseEntitiesArray().begin(), sphereColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) &&
				shapePool->HasComponent(entity) &&
				shapePool->GetComponent(entity).shape != nullptr &&
				(sphereColliderPool->IsFlagSet(entity, UPDATE_FLAG) || transformPool->IsFlagSet(entity, CHANGED_FLAG)))
			{
				auto& sphereColliderComponent = sphereColliderPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				auto& shapeComponent = shapePool->GetComponent(entity);
				auto indexDC = sphereColliderPool->GetIndex(entity);

				glm::vec3 origin = transformComponent.fullTransform * glm::vec4(shapeComponent.shape->GetObbOrigin(), 1);
				glm::vec3 extent = transformComponent.fullTransform * glm::vec4(shapeComponent.shape->GetObbOrigin() + shapeComponent.shape->GetObbExtents(), 1);

				float radius = glm::distance(extent, origin);
				sphereColliderComponent.radius = radius;
				sphereColliderComponent.origin = origin;

				glm::mat4 scTransform = glm::translate(origin) * glm::scale(glm::vec3(radius));
				scTransformSsboHandler[indexDC] = scTransform;
				sphereColliderPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);

	//scTransformSsbo->UnMapBuffer();
}