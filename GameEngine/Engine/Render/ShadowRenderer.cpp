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
				RenderDirLightShadowModelInstanced(registry);

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
				resourceManager->GetSsbo("PointLightData")->BindBufferBase(0);
				resourceManager->GetSsbo("TransformData")->BindBufferBase(1);
				auto program = resourceManager->GetProgram("ShadowPoint");
				program->Bind();
				program->SetUniform("u_lightIndex", pointLightIndex);

				RenderPointLightShadowShapes(registry, pointLightComponent);
				RenderPointLightShadowShapesInstanced(registry, pointLightComponent);
				RenderPointLightShadowModel(registry, pointLightComponent);
				RenderPointLightShadowModelInstanced(registry, pointLightComponent);

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
				resourceManager->GetSsbo("SpotLightData")->BindBufferBase(0);
				resourceManager->GetSsbo("TransformData")->BindBufferBase(1);
				auto program = resourceManager->GetProgram("ShadowSpot");
				program->Bind();
				program->SetUniform("u_lightIndex", spotLightIndex);

				RenderSpotLightShadowShapes(registry, spotLightComponent);
				RenderSpotLightShadowShapesInstanced(registry, spotLightComponent);
				RenderSpotLightShadowModel(registry, spotLightComponent);
				RenderSpotLightShadowModelInstanced(registry, spotLightComponent);

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

				//Itt nem kell resourceManager->GetSsbo("ShapeData")->BindBufferBase(3); mert eldõnthetõ cpu oldalt
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
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	if (!transformPool || !shapePool)
		return;

	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;
		geometry->ClearShadowInstances();
	}

	std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) &&
				shapePool->GetComponent(entity).shape != nullptr)
			{
				auto& shapeComponent = shapePool->GetComponent(entity);
				auto transformIndex = transformPool->GetIndex(entity);

				if (shapeComponent.isInstanced && shapeComponent.castShadow)
					shapeComponent.shape->AddShadowInstanceID(transformIndex);
			}
		}
	);

	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;
		geometry->UpdateShadowInstanceSsbo();
	}
	
	auto program = resourceManager->GetProgram("ShadowDir");
	program->SetUniform("u_renderMode", (GLuint)1);
	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;

		if (geometry->GetShadowInstances().size() > 0)
		{
			geometry->GetShadowInstanceSsbo()->BindBufferBase(2);
			geometry->Bind();
			glDrawElementsInstanced(GL_TRIANGLES, geometry->GetIndexCount(), GL_UNSIGNED_INT, nullptr, geometry->GetShadowInstances().size());
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

void ShadowRenderer::RenderDirLightShadowModelInstanced(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto modelManager = ModelManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();

	if (!transformPool || !modelPool)
		return;

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;
		model->ClearShadowInstances();
	}

	std::for_each(std::execution::seq, modelPool->GetDenseEntitiesArray().begin(), modelPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) &&
				modelPool->GetComponent(entity).model != nullptr)
			{
				auto& modelComponent = modelPool->GetComponent(entity);
				auto transformIndex = transformPool->GetIndex(entity);

				if (modelComponent.isInstanced && modelComponent.castShadow)
					modelComponent.model->AddShadowInstanceID(transformIndex);
			}
		}
	);

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;
		model->UpdateShadowInstanceSsbo();
	}

	auto program = resourceManager->GetProgram("ShadowDir");
	program->SetUniform("u_renderMode", (GLuint)3);

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;

		if (model->GetShadowInstances().size() > 0)
		{
			model->GetShadowInstanceSsbo()->BindBufferBase(2);

			model->Bind();
			glDrawElementsInstanced(GL_TRIANGLES, model->GetIndexCount(), GL_UNSIGNED_INT, nullptr, model->GetShadowInstances().size());
			model->UnBind();
		}
	}
}

void ShadowRenderer::RenderPointLightShadowShapes(std::shared_ptr<Registry> registry, const PointLightComponent& pointLight)
{
	auto shapePool = registry->GetComponentPool<ShapeComponent>();
	if (!shapePool)
		return;

	auto resourceManager = ResourceManager::Instance();
	auto program = resourceManager->GetProgram("ShadowPoint");
	program->SetUniform("u_renderMode", (GLuint)0);

	std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (registry->HasComponent<ShapeComponent>(entity) &&
				registry->GetComponent<ShapeComponent>(entity).shape != nullptr &&
				registry->HasComponent<TransformComponent>(entity))
			{
				auto& shapeComponent = shapePool->GetComponent(entity);
				auto transformIndex = registry->GetIndex<TransformComponent>(entity);

				//Itt nem kell resourceManager->GetSsbo("ShapeData")->BindBufferBase(3); mert eldõnthetõ cpu oldalt
				if (pointLight.visibleEntities[entity] && !shapeComponent.isInstanced && shapeComponent.castShadow)
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

void ShadowRenderer::RenderPointLightShadowShapesInstanced(std::shared_ptr<Registry> registry, const PointLightComponent& pointLight)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	if (!transformPool || !shapePool)
		return;

	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;
		geometry->ClearShadowInstances();
	}

	std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) &&
				shapePool->GetComponent(entity).shape != nullptr)
			{
				auto& transformComponent = transformPool->GetComponent(entity);
				auto& shapeComponent = shapePool->GetComponent(entity);
				auto transformIndex = transformPool->GetIndex(entity);
				auto shapeIndex = shapePool->GetIndex(entity);

				if (pointLight.visibleEntities[entity] && shapeComponent.isInstanced && shapeComponent.castShadow)
					shapeComponent.shape->AddShadowInstanceID(transformIndex);
			}
		}
	);

	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;
		geometry->UpdateShadowInstanceSsbo();
	}

	auto program = resourceManager->GetProgram("ShadowPoint");
	program->SetUniform("u_renderMode", (GLuint)1);

	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;

		if (geometry->GetShadowInstances().size() > 0)
		{
			geometry->GetShadowInstanceSsbo()->BindBufferBase(2);
			geometry->Bind();
			glDrawElementsInstanced(GL_TRIANGLES, geometry->GetIndexCount(), GL_UNSIGNED_INT, nullptr, geometry->GetShadowInstances().size());
			geometry->UnBind();
		}
	}
}

void ShadowRenderer::RenderPointLightShadowModel(std::shared_ptr<Registry> registry, const PointLightComponent& pointLight)
{
	auto modelPool = registry->GetComponentPool<ModelComponent>();
	if (!modelPool)
		return;

	auto resourceManager = ResourceManager::Instance();
	auto program = resourceManager->GetProgram("ShadowPoint");
	program->SetUniform("u_renderMode", (GLuint)2);

	std::for_each(std::execution::seq, modelPool->GetDenseEntitiesArray().begin(), modelPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (registry->HasComponent<TransformComponent>(entity) &&
				registry->GetComponent<ModelComponent>(entity).model != nullptr &&
				!registry->HasComponent<AnimationComponent>(entity))
			{
				auto& modelComponent = modelPool->GetComponent(entity);
				auto transformIndex = registry->GetIndex<TransformComponent>(entity);

				if (pointLight.visibleEntities[entity] && !modelComponent.isInstanced && modelComponent.castShadow)
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

void ShadowRenderer::RenderPointLightShadowModelInstanced(std::shared_ptr<Registry> registry, const PointLightComponent& pointLight)
{
	auto resourceManager = ResourceManager::Instance();
	auto modelManager = ModelManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();

	if (!transformPool || !modelPool)
		return;

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;
		model->ClearShadowInstances();
	}

	std::for_each(std::execution::seq, modelPool->GetDenseEntitiesArray().begin(), modelPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) &&
				modelPool->GetComponent(entity).model != nullptr)
			{
				auto& transformComponent = transformPool->GetComponent(entity);
				auto& modelComponent = modelPool->GetComponent(entity);
				auto transformIndex = transformPool->GetIndex(entity);

				if (pointLight.visibleEntities[entity] && modelComponent.isInstanced && modelComponent.castShadow)
					modelComponent.model->AddShadowInstanceID(transformIndex);
			}
		}
	);

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;
		model->UpdateShadowInstanceSsbo();
	}

	auto program = resourceManager->GetProgram("ShadowPoint");
	program->SetUniform("u_renderMode", (GLuint)3);

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;

		if (model->GetShadowInstances().size() > 0)
		{
			model->GetShadowInstanceSsbo()->BindBufferBase(2);

			model->Bind();
			glDrawElementsInstanced(GL_TRIANGLES, model->GetIndexCount(), GL_UNSIGNED_INT, nullptr, model->GetShadowInstances().size());
			model->UnBind();
		}
	}
}

void ShadowRenderer::RenderSpotLightShadowShapes(std::shared_ptr<Registry> registry, const SpotLightComponent& spotLight)
{
	auto shapePool = registry->GetComponentPool<ShapeComponent>();
	if (!shapePool)
		return;

	auto resourceManager = ResourceManager::Instance();
	auto program = resourceManager->GetProgram("ShadowSpot");
	program->SetUniform("u_renderMode", (GLuint)0);

	int counter = 0;

	std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (registry->HasComponent<ShapeComponent>(entity) &&
				registry->GetComponent<ShapeComponent>(entity).shape != nullptr &&
				registry->HasComponent<TransformComponent>(entity))
			{
				auto& shapeComponent = shapePool->GetComponent(entity);
				auto transformIndex = registry->GetIndex<TransformComponent>(entity);

				//Itt nem kell resourceManager->GetSsbo("ShapeData")->BindBufferBase(3); mert eldõnthetõ cpu oldalt
				if (spotLight.visibleEntities[entity] && !shapeComponent.isInstanced && shapeComponent.castShadow)
				{
					program->SetUniform("u_transformIndex", transformIndex);
					shapeComponent.shape->Bind();
					glDrawElements(GL_TRIANGLES, shapeComponent.shape->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
					shapeComponent.shape->UnBind();

					counter++;
				}
			}
		}
	);

	std::cout << counter << std::endl;
}

void ShadowRenderer::RenderSpotLightShadowShapesInstanced(std::shared_ptr<Registry> registry, const SpotLightComponent& spotLight)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	if (!transformPool || !shapePool)
		return;

	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;
		geometry->ClearShadowInstances();
	}

	std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) &&
				shapePool->GetComponent(entity).shape != nullptr)
			{
				auto& shapeComponent = shapePool->GetComponent(entity);
				auto transformIndex = transformPool->GetIndex(entity);

				if (spotLight.visibleEntities[entity] && shapeComponent.isInstanced && shapeComponent.castShadow)
					shapeComponent.shape->AddShadowInstanceID(transformIndex);
			}
		}
	);

	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;
		geometry->UpdateShadowInstanceSsbo();
	}

	auto program = resourceManager->GetProgram("ShadowSpot");
	program->SetUniform("u_renderMode", (GLuint)1);

	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;

		if (geometry->GetShadowInstances().size() > 0)
		{
			geometry->GetShadowInstanceSsbo()->BindBufferBase(2);
			geometry->Bind();
			glDrawElementsInstanced(GL_TRIANGLES, geometry->GetIndexCount(), GL_UNSIGNED_INT, nullptr, geometry->GetShadowInstances().size());
			geometry->UnBind();
		}
	}
}

void ShadowRenderer::RenderSpotLightShadowModel(std::shared_ptr<Registry> registry, const SpotLightComponent& spotLight)
{
	auto modelPool = registry->GetComponentPool<ModelComponent>();
	if (!modelPool)
		return;

	auto resourceManager = ResourceManager::Instance();
	auto program = resourceManager->GetProgram("ShadowSpot");
	program->SetUniform("u_renderMode", (GLuint)2);

	std::for_each(std::execution::seq, modelPool->GetDenseEntitiesArray().begin(), modelPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (registry->HasComponent<TransformComponent>(entity) &&
				registry->GetComponent<ModelComponent>(entity).model != nullptr &&
				!registry->HasComponent<AnimationComponent>(entity))
			{
				auto& modelComponent = modelPool->GetComponent(entity);
				auto transformIndex = registry->GetIndex<TransformComponent>(entity);

				if (spotLight.visibleEntities[entity] && !modelComponent.isInstanced && modelComponent.castShadow)
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

void ShadowRenderer::RenderSpotLightShadowModelInstanced(std::shared_ptr<Registry> registry, const SpotLightComponent& spotLight)
{
	auto resourceManager = ResourceManager::Instance();
	auto modelManager = ModelManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();

	if (!transformPool || !modelPool)
		return;

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;
		model->ClearShadowInstances();
	}

	std::for_each(std::execution::seq, modelPool->GetDenseEntitiesArray().begin(), modelPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) &&
				modelPool->GetComponent(entity).model != nullptr)
			{
				auto& transformComponent = transformPool->GetComponent(entity);
				auto& modelComponent = modelPool->GetComponent(entity);
				auto transformIndex = transformPool->GetIndex(entity);

				if (spotLight.visibleEntities[entity] && modelComponent.isInstanced && modelComponent.castShadow)
					modelComponent.model->AddShadowInstanceID(transformIndex);
			}
		}
	);

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;
		model->UpdateShadowInstanceSsbo();
	}

	auto program = resourceManager->GetProgram("ShadowSpot");
	program->SetUniform("u_renderMode", (GLuint)3);

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;

		if (model->GetShadowInstances().size() > 0)
		{
			model->GetShadowInstanceSsbo()->BindBufferBase(2);

			model->Bind();
			glDrawElementsInstanced(GL_TRIANGLES, model->GetIndexCount(), GL_UNSIGNED_INT, nullptr, model->GetShadowInstances().size());
			model->UnBind();
		}
	}
}