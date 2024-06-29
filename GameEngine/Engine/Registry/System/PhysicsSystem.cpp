#include "PhysicsSystem.h"

glm::vec3 PhysicsSystem::gravity = glm::vec3{ 0, -9.81, 0 };

void PhysicsSystem::OnStart(std::shared_ptr<Registry> registry)
{

}

void PhysicsSystem::OnUpdate(std::shared_ptr<Registry> registry, float deltaTime)
{
	auto resourceManager = ResourceManager::Instance();
	auto rigidbodyPool = registry->GetComponentPool<RigidbodyComponent>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();

	if (!rigidbodyPool || !transformPool)
		return;

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

nlohmann::json PhysicsSystem::Serialize(Registry* registry, Entity entity)
{
	auto& rigidbodyComponent = registry->GetComponent<RigidbodyComponent>(entity);

	nlohmann::json data;
	data["isStatic"] = rigidbodyComponent.isStatic;
	data["mass"] = rigidbodyComponent.mass;

	return data;
}

void PhysicsSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& rigidbodyComponent = registry->GetComponent<RigidbodyComponent>(entity);

	rigidbodyComponent.isStatic = data["isStatic"];
	rigidbodyComponent.mass = data["mass"];

	registry->SetFlag<RigidbodyComponent>(entity, UPDATE_FLAG);
}