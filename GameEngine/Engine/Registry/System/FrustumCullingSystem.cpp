#include "FrustumCullingSystem.h"

void FrustumCullingSystem::OnStart(std::shared_ptr<Registry> registry, std::shared_ptr<Camera> camera)
{

}

void FrustumCullingSystem::OnUpdate(std::shared_ptr<Registry> registry, std::shared_ptr<Camera> camera)
{
	auto defaultColliderPool = registry->GetComponentPool<DefaultCollider>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();

	if (!transformPool || !defaultColliderPool)
		return;

	DefaultCollider frustumCollider;

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
				glm::vec4 pt = camera->GetViewProjInv() * glm::vec4(x_ncd, y_ncd, z_ncd, 1.0f);
				pt /= pt.w;
				frustumCollider.positions[index++] = pt;
			}
		}
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
				if (CollisionTester::Test(&frustumCollider, &defaultColliderComponent, simplex))
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