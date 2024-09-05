#include "ShadowCullingSystem.h"
#include "Collision/AABB.h"

void ShadowCullingSystem::OnStart(std::shared_ptr<Registry> registry)
{

}

void ShadowCullingSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	PointLightShadowCulling(registry);
}

void ShadowCullingSystem::PointLightShadowCulling(std::shared_ptr<Registry> registry)
{
	auto defaultColliderPool = registry->GetComponentPool<DefaultCollider>();
	auto pointLightPool = registry->GetComponentPool<PointLightComponent>();

	if (!pointLightPool || !defaultColliderPool)
		return;
	
	/*
	std::for_each(std::execution::par, pointLightPool->GetDenseEntitiesArray().begin(), pointLightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entityLight) -> void {
			auto& pointLightComponent = pointLightPool->GetComponent(entityLight);
			pointLightComponent.visibleEntities.clear();

			SphereColliderGjk pointLightCollider;
			pointLightCollider.origin = pointLightComponent.position;
			pointLightCollider.radius = pointLightComponent.farPlane;

			std::for_each(std::execution::seq, defaultColliderPool->GetDenseEntitiesArray().begin(), defaultColliderPool->GetDenseEntitiesArray().end(),
				[&](const Entity& entity) -> void {
					auto& defaultColliderComponent = defaultColliderPool->GetComponent(entity);

					Simplex simplex;
					if (CollisionTester::Test(&pointLightCollider, &defaultColliderComponent, simplex))
						pointLightComponent.visibleEntities.insert(entity);
				}
			);
		}
	);
	*/

	std::for_each(std::execution::seq, pointLightPool->GetDenseEntitiesArray().begin(), pointLightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entityLight) -> void {
			auto& pointLightComponent = pointLightPool->GetComponent(entityLight);
			pointLightComponent.visibleEntities.clear();
		}
	);

	std::mutex mutex;
	std::for_each(std::execution::par, defaultColliderPool->GetDenseEntitiesArray().begin(), defaultColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			auto& defaultColliderComponent = defaultColliderPool->GetComponent(entity);

			std::for_each(std::execution::seq, pointLightPool->GetDenseEntitiesArray().begin(), pointLightPool->GetDenseEntitiesArray().end(),
				[&](const Entity& entityLight) -> void {
					auto& pointLightComponent = pointLightPool->GetComponent(entityLight);

					if (pointLightComponent.useShadow)
					{
						SphereColliderGjk pointLightCollider;
						pointLightCollider.origin = pointLightComponent.position;
						pointLightCollider.radius = pointLightComponent.farPlane;

						glm::vec3 aabbMin = pointLightCollider.origin - glm::vec3(pointLightCollider.radius);
						glm::vec3 aabbMax = pointLightCollider.origin + glm::vec3(pointLightCollider.radius);

						Simplex simplex;
						if (AABB::Test(defaultColliderComponent.aabbMin, defaultColliderComponent.aabbMax, aabbMin, aabbMax) && CollisionTester::Test(&pointLightCollider, &defaultColliderComponent, simplex))
						{
							std::lock_guard<std::mutex> lock_guard(mutex);
							pointLightComponent.visibleEntities.insert(entityLight);
						}
					}
				}
			);
		}
	);
}