#include "ShadowCullingSystem.h"
#include "Collision/AABB.h"

void ShadowCullingSystem::OnStart(std::shared_ptr<Registry> registry)
{

}

void ShadowCullingSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	PointLightShadowCulling(registry);
	SpotLightShadowCulling(registry);
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

				/*
				pointLightComponent.visibleEntitiesID.clear();
				for (int i = 0; i < pointLightComponent.visibleEntities.size(); ++i)
				{
					if (pointLightComponent.visibleEntities[i])
						pointLightComponent.visibleEntitiesID.push_back(i);
				}
				*/
			}
			
		}
	);
}

void ShadowCullingSystem::SpotLightShadowCulling(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto defaultColliderPool = registry->GetComponentPool<DefaultCollider>();
	auto spotLightPool = registry->GetComponentPool<SpotLightComponent>();
	auto cone = resourceManager->GetGeometry("Cone");

	if (!spotLightPool || !defaultColliderPool)
		return;

	std::for_each(std::execution::seq, spotLightPool->GetDenseEntitiesArray().begin(), spotLightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entityLight) -> void {
			auto& spotLightComponent = spotLightPool->GetComponent(entityLight);

			if (spotLightComponent.useShadow)
			{
				spotLightComponent.visibleEntities.clear();
				spotLightComponent.visibleEntities.resize(registry->GetEntityCount());

				ConvexColliderGjk spotLightCollider;
				spotLightCollider.positions.reserve(cone->GetSurfacePoints().size());

				glm::vec3 lightAabbMin = glm::vec3(std::numeric_limits<float>::max());
				glm::vec3 lightAabbMax = glm::vec3(std::numeric_limits<float>::lowest());

				for (const auto& position : cone->GetSurfacePoints())
				{
					glm::vec3 worldPos = spotLightComponent.proxyTransform * glm::vec4(position, 1);
					lightAabbMin = glm::min(lightAabbMin, worldPos);
					lightAabbMax = glm::max(lightAabbMax, worldPos);
					spotLightCollider.positions.emplace_back(worldPos);
				}

				std::for_each(std::execution::par, defaultColliderPool->GetDenseEntitiesArray().begin(), defaultColliderPool->GetDenseEntitiesArray().end(),
					[&](const Entity& entity) -> void {
						auto& defaultColliderComponent = defaultColliderPool->GetComponent(entity);

						Simplex simplex;
						if (AABB::Test(defaultColliderComponent.aabbMin, defaultColliderComponent.aabbMax, lightAabbMin, lightAabbMax) && CollisionTester::Test(&spotLightCollider, &defaultColliderComponent, simplex))
						{
							spotLightComponent.visibleEntities[entity] = true;
						}
					}
				);

				/*
				spotLightComponent.visibleEntitiesID.clear();
				for (int i = 0; i < spotLightComponent.visibleEntities.size(); ++i)
				{
					if (spotLightComponent.visibleEntities[i])
						spotLightComponent.visibleEntitiesID.push_back(i);
				}
				*/
			}
		}
	);

}