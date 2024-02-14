#include "FrustumCullingSystem.h"

void FrustumCullingSystem::OnStart(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager)
{

}

void FrustumCullingSystem::OnUpdate(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager)
{
	auto defaultColliderPool = registry->GetComponentPool<DefaultCollider>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	auto& camera = Renderer::Instance()->m_Camera;
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
			if (transformPool->HasComponent(entity) &&
				shapePool->HasComponent(entity) &&
				shapePool->GetComponent(entity).shape != nullptr)			
			{
				auto& defaultColliderComponent = defaultColliderPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				auto& shapeComponent = shapePool->GetComponent(entity);

				Simplex simplex;
				if (CollisionTester::Test(&frustumCollider, &defaultColliderComponent, simplex))
					shapeComponent.toRender = true;
				else
					shapeComponent.toRender = false;
			}
		}
	);
}