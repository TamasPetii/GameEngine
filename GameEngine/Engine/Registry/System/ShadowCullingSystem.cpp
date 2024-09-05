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
	
	std::for_each(std::execution::seq, pointLightPool->GetDenseEntitiesArray().begin(), pointLightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entityLight) -> void {
			auto& pointLightComponent = pointLightPool->GetComponent(entityLight);
			
			if (pointLightComponent.useShadow)
			{
				pointLightComponent.visibleEntities.clear();
				pointLightComponent.visibleEntities.resize(registry->GetEntityCount());

				SphereColliderGjk pointLightCollider;
				pointLightCollider.origin = pointLightComponent.position;
				pointLightCollider.radius = pointLightComponent.farPlane;

				glm::vec3 lightAabbMin = pointLightCollider.origin - pointLightCollider.radius;
				glm::vec3 lightAabbMax = pointLightCollider.origin + pointLightCollider.radius;

				std::for_each(std::execution::par, defaultColliderPool->GetDenseEntitiesArray().begin(), defaultColliderPool->GetDenseEntitiesArray().end(),
					[&](const Entity& entity) -> void {
						auto& defaultColliderComponent = defaultColliderPool->GetComponent(entity);

						Simplex simplex;
						if (AABB::Test(defaultColliderComponent.aabbMin, defaultColliderComponent.aabbMax, lightAabbMin, lightAabbMax) && CollisionTester::Test(&pointLightCollider, &defaultColliderComponent, simplex))
						{
							pointLightComponent.visibleEntities[entity] = true;
						}
					}
				);
			}
			
		}
	);
}