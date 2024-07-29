#include "ShadowRenderer.h"

void ShadowRenderer::Render(std::shared_ptr<Registry> registry)
{
	RenderDirLightShadows(registry);
	RenderPointLightShadows(registry);
	RenderSpotLightShadows(registry);
};

void ShadowRenderer::RenderDirLightShadows(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto dirlightPool = registry->GetComponentPool<DirlightComponent>();

	if (!dirlightPool)
		return;

	std::for_each(std::execution::seq, dirlightPool->GetDenseEntitiesArray().begin(), dirlightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entityLight) -> void {
			auto& dirlightComponent = dirlightPool->GetComponent(entityLight);
			auto dirlightIndex = dirlightPool->GetIndex(entityLight);

			if (dirlightComponent.useShadow)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, dirlightComponent.shadowFramebuffer);
				glViewport(0, 0, dirlightComponent.shadowSize, dirlightComponent.shadowSize);
				glClear(GL_DEPTH_BUFFER_BIT);
				resourceManager->GetSsbo("TransformData")->BindBufferBase(0);
				resourceManager->GetSsbo("DirLightData")->BindBufferBase(1);
				auto program = resourceManager->GetProgram("ShadowDir");
				program->Bind();
				program->SetUniform("u_lightIndex", dirlightIndex);

				auto shapePool = registry->GetComponentPool<ShapeComponent>();
				std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
					[&](const Entity& entityShape) -> void {
						if (registry->HasComponent<ShapeComponent>(entityShape) &&
							registry->GetComponent<ShapeComponent>(entityShape).shape != nullptr &&
							registry->HasComponent<TransformComponent>(entityShape))
						{
							auto& shapeComponent = shapePool->GetComponent(entityShape);
							auto transformIndex = registry->GetIndex<TransformComponent>(entityShape);
							auto shapeIndex = registry->GetIndex<ShapeComponent>(entityShape);

							if (shapeComponent.castShadow)
							{
								program->SetUniform("u_transformIndex", transformIndex);
								shapeComponent.shape->Bind();
								glDrawElements(GL_TRIANGLES, shapeComponent.shape->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
								shapeComponent.shape->UnBind();
							}
						}
					}
				);

				program->UnBind();
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}
	);
}

void ShadowRenderer::RenderPointLightShadows(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto pointLightPool = registry->GetComponentPool<PointLightComponent>();

	if (!pointLightPool)
		return;

	std::for_each(std::execution::seq, pointLightPool->GetDenseEntitiesArray().begin(), pointLightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entityLight) -> void {
			auto& pointLightComponent = pointLightPool->GetComponent(entityLight);
			auto pointLightIndex = pointLightPool->GetIndex(entityLight);

			if (pointLightComponent.useShadow)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, pointLightComponent.shadowFramebuffer);
				glViewport(0, 0, pointLightComponent.shadowSize, pointLightComponent.shadowSize);
				glClear(GL_DEPTH_BUFFER_BIT);
				resourceManager->GetSsbo("TransformData")->BindBufferBase(0);
				resourceManager->GetSsbo("PointLightData")->BindBufferBase(1);
				auto program = resourceManager->GetProgram("ShadowPoint");
				program->Bind();
				program->SetUniform("u_lightIndex", pointLightIndex);

				auto shapePool = registry->GetComponentPool<ShapeComponent>();
				std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
					[&](const Entity& entityShape) -> void {
						if (registry->HasComponent<ShapeComponent>(entityShape) &&
							registry->GetComponent<ShapeComponent>(entityShape).shape != nullptr &&
							registry->HasComponent<TransformComponent>(entityShape))
						{
							auto& shapeComponent = shapePool->GetComponent(entityShape);
							auto transformIndex = registry->GetIndex<TransformComponent>(entityShape);
							auto shapeIndex = registry->GetIndex<ShapeComponent>(entityShape);

							if (shapeComponent.castShadow)
							{
								program->SetUniform("u_transformIndex", transformIndex);
								shapeComponent.shape->Bind();
								glDrawElements(GL_TRIANGLES, shapeComponent.shape->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
								shapeComponent.shape->UnBind();
							}
						}
					}
				);

				program->UnBind();
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}
	);
}

void ShadowRenderer::RenderSpotLightShadows(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto spotLightPool = registry->GetComponentPool<SpotLightComponent>();

	if (!spotLightPool)
		return;

	std::for_each(std::execution::seq, spotLightPool->GetDenseEntitiesArray().begin(), spotLightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entityLight) -> void {
			auto& spotLightComponent = spotLightPool->GetComponent(entityLight);
			auto spotLightIndex = spotLightPool->GetIndex(entityLight);

			if (spotLightComponent.useShadow)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, spotLightComponent.shadowFramebuffer);
				glViewport(0, 0, spotLightComponent.shadowSize, spotLightComponent.shadowSize);
				glClear(GL_DEPTH_BUFFER_BIT);
				resourceManager->GetSsbo("TransformData")->BindBufferBase(0);
				resourceManager->GetSsbo("SpotLightData")->BindBufferBase(1);
				auto program = resourceManager->GetProgram("ShadowSpot");
				program->Bind();
				program->SetUniform("u_lightIndex", spotLightIndex);

				auto shapePool = registry->GetComponentPool<ShapeComponent>();
				std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
					[&](const Entity& entityShape) -> void {
						if (registry->HasComponent<ShapeComponent>(entityShape) &&
							registry->GetComponent<ShapeComponent>(entityShape).shape != nullptr &&
							registry->HasComponent<TransformComponent>(entityShape))
						{
							auto& shapeComponent = shapePool->GetComponent(entityShape);
							auto transformIndex = registry->GetIndex<TransformComponent>(entityShape);
							auto shapeIndex = registry->GetIndex<ShapeComponent>(entityShape);

							if (shapeComponent.castShadow)
							{
								program->SetUniform("u_transformIndex", transformIndex);
								shapeComponent.shape->Bind();
								glDrawElements(GL_TRIANGLES, shapeComponent.shape->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
								shapeComponent.shape->UnBind();
							}
						}
					}
				);

				program->UnBind();
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}
	);
}
