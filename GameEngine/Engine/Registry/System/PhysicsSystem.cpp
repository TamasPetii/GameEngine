#include "PhysicsSystem.h"

void PhysicsSystem::OnStart(std::shared_ptr<Registry> registry)
{

}

void PhysicsSystem::OnUpdate(std::shared_ptr<Registry> registry, float deltaTime)
{
	auto resourceManager = ResourceManager::Instance();
	auto rigidbodyPool = registry->GetComponentPool<RigidbodyComponent>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();

	std::for_each(std::execution::par, rigidbodyPool->GetDenseEntitiesArray().begin(), rigidbodyPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (registry->HasComponent<TransformComponent>(entity) && !registry->GetComponent<RigidbodyComponent>(entity).isStatic)
			{
				auto& rigidbodyComponent = rigidbodyPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				
				rigidbodyComponent.position = transformComponent.translate;
				rigidbodyComponent.force += rigidbodyComponent.mass * gravity;
				rigidbodyComponent.velocity += rigidbodyComponent.force / rigidbodyComponent.mass * deltaTime;
				rigidbodyComponent.position += rigidbodyComponent.velocity * deltaTime;
				rigidbodyComponent.force = glm::vec3(0, 0, 0);
				transformComponent.translate = rigidbodyComponent.position;

				transformPool->SetFlag(entity, UPDATE_FLAG);
			}
		}
	);
}

void PhysicsSystem::OnEnd(std::shared_ptr<Registry> registry)
{

}
