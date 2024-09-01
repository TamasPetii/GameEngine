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

				auto program = resourceManager->GetProgram("ShadowDir");
				program->Bind();
				program->SetUniform("u_lightIndex", dirlightIndex);
				resourceManager->GetSsbo("DirLightData")->BindBufferBase(0);
				resourceManager->GetSsbo("TransformData")->BindBufferBase(1);

				RenderDirLightShadowShapes(registry);
				RenderDirLightShadowShapesInstanced(registry);
				RenderDirLightShadowModel(registry);
				RenderDirLightShadowModelInstance(registry);

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

void ShadowRenderer::RenderDirLightShadowShapes(std::shared_ptr<Registry> registry)
{
	auto shapePool = registry->GetComponentPool<ShapeComponent>();
	if (!shapePool)
		return;

	auto resourceManager = ResourceManager::Instance();
	auto program = resourceManager->GetProgram("ShadowDir");
	program->SetUniform("u_renderMode", (GLuint)0);

	std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (registry->HasComponent<ShapeComponent>(entity) &&
				registry->GetComponent<ShapeComponent>(entity).shape != nullptr &&
				registry->HasComponent<TransformComponent>(entity))
			{
				auto& shapeComponent = shapePool->GetComponent(entity);
				auto transformIndex = registry->GetIndex<TransformComponent>(entity);

				//Itt nem kell resourceManager->GetSsbo("ShapeData")->BindBufferBase(3); mert eld�nthet� cpu oldalt
				if (!shapeComponent.isInstanced && shapeComponent.castShadow)
				{
					program->SetUniform("u_transformIndex", transformIndex);
					shapeComponent.shape->Bind();
					glDrawElements(GL_TRIANGLES, shapeComponent.shape->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
					shapeComponent.shape->UnBind();
				}
			}
		}
	);
}

void ShadowRenderer::RenderDirLightShadowShapesInstanced(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto program = resourceManager->GetProgram("ShadowDir");
	program->SetUniform("u_renderMode", (GLuint)1);
	resourceManager->GetSsbo("ShapeData")->BindBufferBase(3);

	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;

		if (geometry->GetInstances().size() > 0)
		{
			geometry->GetInstanceSsbo()->BindBufferBase(2);
			geometry->Bind();
			glDrawElementsInstanced(GL_TRIANGLES, geometry->GetIndexCount(), GL_UNSIGNED_INT, nullptr, geometry->GetInstances().size());
			geometry->UnBind();
		}
	}
}

void ShadowRenderer::RenderDirLightShadowModel(std::shared_ptr<Registry> registry)
{
	auto modelPool = registry->GetComponentPool<ModelComponent>();
	if (!modelPool)
		return;

	auto resourceManager = ResourceManager::Instance();
	auto program = resourceManager->GetProgram("ShadowDir");
	program->SetUniform("u_renderMode", (GLuint)2);

	std::for_each(std::execution::seq, modelPool->GetDenseEntitiesArray().begin(), modelPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (registry->HasComponent<TransformComponent>(entity) && !registry->HasComponent<AnimationComponent>(entity))
			{
				auto& modelComponent = modelPool->GetComponent(entity);
				auto transformIndex = registry->GetIndex<TransformComponent>(entity);

				if (modelComponent.model && !modelComponent.isInstanced && modelComponent.castShadow)
				{
					program->SetUniform("u_transformIndex", transformIndex);
					modelComponent.model->Bind();
					glDrawElements(GL_TRIANGLES, modelComponent.model->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
					modelComponent.model->UnBind();
				}
			}
		}
	);
}

void ShadowRenderer::RenderDirLightShadowModelInstance(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto modelManager = ModelManager::Instance();

	auto program = resourceManager->GetProgram("ShadowDir");
	program->SetUniform("u_renderMode", (GLuint)3);
	resourceManager->GetSsbo("ModelData")->BindBufferBase(3);

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;
		if (model->GetInstanceCount() > 0)
		{
			model->GetInstanceSsbo()->BindBufferBase(2);

			model->Bind();
			glDrawElementsInstanced(GL_TRIANGLES, model->GetIndexCount(), GL_UNSIGNED_INT, nullptr, model->GetInstanceCount());
			model->UnBind();
		}
	}
}