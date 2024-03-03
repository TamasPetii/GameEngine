#include "CollisionResponseSystem.h"

void CollisionResponseSystem::OnStart(std::shared_ptr<Registry> registry)
{

}

void CollisionResponseSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto defaultColliderPool = registry->GetComponentPool<DefaultCollider>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();
	auto rigidbodyPool = registry->GetComponentPool<RigidbodyComponent>();
	
	std::for_each(std::execution::par, CollisionSystem::collisionData.begin(), CollisionSystem::collisionData.end(),
		[&](auto& data) -> void {
			Entity entityA = std::get<0>(data);
			Entity entityB = std::get<1>(data);
			Simplex& simplex = std::get<2>(data);

			if (registry->HasComponent<RigidbodyComponent>(entityA) && registry->HasComponent<RigidbodyComponent>(entityB))
			{
				auto& rigidbodyA = rigidbodyPool->GetComponent(entityA);
				auto& rigidbodyB = rigidbodyPool->GetComponent(entityB);
				auto& transformComponentA = transformPool->GetComponent(entityA);
				auto& transformComponentB = transformPool->GetComponent(entityB);
				auto& defaultColliderA = defaultColliderPool->GetComponent(entityA);
				auto& defaultColliderB = defaultColliderPool->GetComponent(entityB);

				if (!rigidbodyA.isStatic)
				{
					auto repsond = CollisionResponder::Resolve(&defaultColliderA, &defaultColliderB, simplex);
					transformComponentA.translate -= repsond.normal * repsond.depth;
					rigidbodyA.velocity = glm::vec3(0, 0, 0);
					transformPool->SetFlag(entityA, UPDATE_FLAG);
					rigidbodyPool->SetFlag(entityA, UPDATE_FLAG);
				}
			}
		}
	);
}