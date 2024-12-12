#include "FrustumCullingSystem.h"

void FrustumCullingSystem::OnStart(std::shared_ptr<Registry> registry)
{

}

void FrustumCullingSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto cameraPool = registry->GetComponentPool<CameraComponent>();

	if (!cameraPool)
		return;

	auto& cameraComponent = CameraSystem::GetMainCamera(registry);

	DefaultCollider frustumCollider;
	glm::vec3 cameraAabbMin = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 cameraAabbMax = glm::vec3(std::numeric_limits<float>::lowest());

	int index = 0;
	for (int x = 0; x < 2; ++x)
	{
		for (int y = 0; y < 2; ++y)
		{
			for (int z = 0; z < 2; ++z)
			{
				float x_ncd = 2.0f * x - 1.0f;
				float y_ncd = 2.0f * y - 1.0f;
				float z_ncd = 2.0f * z - 1.0f;
				glm::vec4 pt = cameraComponent.viewProjInv * glm::vec4(x_ncd, y_ncd, z_ncd, 1.0f);
				pt /= pt.w;
				frustumCollider.positions[index++] = pt;
				cameraAabbMin = glm::min(cameraAabbMin, glm::vec3(pt));
				cameraAabbMax = glm::max(cameraAabbMax, glm::vec3(pt));
			}
		}
	}

	frustumCollider.aabbMin = cameraAabbMin;
	frustumCollider.aabbMax = cameraAabbMax;

	GeometryCulling(registry, frustumCollider);
	PointLightCulling(registry, frustumCollider);
	SpotLightCulling(registry, frustumCollider);
}

void FrustumCullingSystem::GeometryCulling(std::shared_ptr<Registry> registry, DefaultCollider& cameraFrustum)
{
	auto defaultColliderPool = registry->GetComponentPool<DefaultCollider>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();

	if (!transformPool || !defaultColliderPool)
		return;

	std::array<DefaultCollider, 4> lodFrustums;

	for (int i = 0; i < 4; ++i)
	{
		auto& cameraComponent = CameraSystem::GetMainCamera(registry);
		glm::mat4 cameraProj = glm::perspective(glm::radians(cameraComponent.fov), cameraComponent.width / cameraComponent.height, i == 0 ? cameraComponent.nearPlane : GlobalSettings::LodDistances[i - 1], GlobalSettings::LodDistances[i]);
		glm::mat4 cameraView = cameraComponent.view;
		glm::mat4 viewProjInv = glm::inverse(cameraProj * cameraView);

		DefaultCollider frustumCollider;
		glm::vec3 cameraAabbMin = glm::vec3(std::numeric_limits<float>::max());
		glm::vec3 cameraAabbMax = glm::vec3(std::numeric_limits<float>::lowest());

		int index = 0;
		for (int x = 0; x < 2; ++x)
		{
			for (int y = 0; y < 2; ++y)
			{
				for (int z = 0; z < 2; ++z)
				{
					float x_ncd = 2.0f * x - 1.0f;
					float y_ncd = 2.0f * y - 1.0f;
					float z_ncd = 2.0f * z - 1.0f;
					glm::vec4 pt = viewProjInv * glm::vec4(x_ncd, y_ncd, z_ncd, 1.0f);
					pt /= pt.w;
					frustumCollider.positions[index++] = pt;
					cameraAabbMin = glm::min(cameraAabbMin, glm::vec3(pt));
					cameraAabbMax = glm::max(cameraAabbMax, glm::vec3(pt));
				}
			}
		}

		frustumCollider.aabbMin = cameraAabbMin;
		frustumCollider.aabbMax = cameraAabbMax;
		lodFrustums[i] = frustumCollider;
	}

	std::for_each(std::execution::par, defaultColliderPool->GetDenseEntitiesArray().begin(), defaultColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			bool hasTransform = transformPool->HasComponent(entity);
			bool hasShape = shapePool && shapePool->HasComponent(entity) && shapePool->GetComponent(entity).shape != nullptr && shapePool->GetComponent(entity).useFrustumCulling;
			bool hasModel = modelPool && modelPool->HasComponent(entity) && modelPool->GetComponent(entity).model != nullptr && modelPool->GetComponent(entity).useFrustumCulling;

			if (hasTransform && (hasShape || hasModel))
			{
				auto& defaultColliderComponent = defaultColliderPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);

				Simplex simplex;
				if (AABB::Test(defaultColliderComponent.aabbMin, defaultColliderComponent.aabbMax, cameraFrustum.aabbMin, cameraFrustum.aabbMax) && CollisionTester::Test(&cameraFrustum, &defaultColliderComponent, simplex))
				{
					if (hasShape)
					{
						auto& shapeComponent = shapePool->GetComponent(entity);
						shapeComponent.toRender = true;
					}
					else if (hasModel)
					{
						auto& modelComponent = modelPool->GetComponent(entity);
						modelComponent.toRender = true;

						if (modelComponent.automaticLod)
						{
							bool insideLod = false;
							for (int i = 0; i < 4; ++i)
							{
								if(AABB::Test(defaultColliderComponent.aabbMin, defaultColliderComponent.aabbMax, lodFrustums[i].aabbMin, lodFrustums[i].aabbMax))
								{
									insideLod = true;
									modelComponent.lodLevel = i;
									break;
								}
							}

							if (!insideLod)
								modelComponent.lodLevel = 3;
						}
					}
				}
				else
				{
					if (hasShape)
					{
						auto& shapeComponent = shapePool->GetComponent(entity);
						shapeComponent.toRender = false;
					}
					else if (hasModel)
					{
						auto& modelComponent = modelPool->GetComponent(entity);
						modelComponent.toRender = false;
					}
				}
			}
		}
	);
}

void FrustumCullingSystem::PointLightCulling(std::shared_ptr<Registry> registry, DefaultCollider& cameraFrustum)
{
	auto defaultColliderPool = registry->GetComponentPool<DefaultCollider>();
	auto pointLightPool = registry->GetComponentPool<PointLightComponent>();

	if (!pointLightPool || !defaultColliderPool)
		return;

	std::for_each(std::execution::par, pointLightPool->GetDenseEntitiesArray().begin(), pointLightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entityLight) -> void {
			auto& pointLightComponent = pointLightPool->GetComponent(entityLight);

			SphereColliderGjk pointLightCollider;
			pointLightCollider.origin = pointLightComponent.position;
			pointLightCollider.radius = pointLightComponent.farPlane;

			glm::vec3 lightAabbMin = pointLightCollider.origin - pointLightCollider.radius;
			glm::vec3 lightAabbMax = pointLightCollider.origin + pointLightCollider.radius;

			Simplex simplex;
			if (AABB::Test(lightAabbMin, lightAabbMax, cameraFrustum.aabbMin, cameraFrustum.aabbMax) && CollisionTester::Test(&pointLightCollider, &cameraFrustum, simplex))
			{
				pointLightComponent.toRender = true;
			}
			else
			{
				pointLightComponent.toRender = false;
			}
		}
	);
}

void FrustumCullingSystem::SpotLightCulling(std::shared_ptr<Registry> registry, DefaultCollider& cameraFrustum)
{
	auto resourceManager = ResourceManager::Instance();
	auto defaultColliderPool = registry->GetComponentPool<DefaultCollider>();
	auto spotLightPool = registry->GetComponentPool<SpotLightComponent>();
	auto cone = resourceManager->GetGeometry("Cone");

	if (!spotLightPool || !defaultColliderPool)
		return;

	std::for_each(std::execution::par, spotLightPool->GetDenseEntitiesArray().begin(), spotLightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entityLight) -> void {
			auto& spotLightComponent = spotLightPool->GetComponent(entityLight);

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

			Simplex simplex;
			if (AABB::Test(lightAabbMin, lightAabbMax, cameraFrustum.aabbMin, cameraFrustum.aabbMax) && CollisionTester::Test(&spotLightCollider, &cameraFrustum, simplex))
			{
				spotLightComponent.toRender = true;
			}
			else
			{
				spotLightComponent.toRender = false;
			}
		}
	);
}
