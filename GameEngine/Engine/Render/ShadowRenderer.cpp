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

				std::vector<char> visibleEntities(registry->GetEntityCount(), 1);

				{
					auto start = std::chrono::high_resolution_clock::now();
					RenderShapesShadow(ShadowType::DIRECTION, registry, visibleEntities);
					auto end = std::chrono::high_resolution_clock::now();
					//std::cout << "RenderPointLightShadowShapes = " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) << std::endl;
				}

				{
					auto start = std::chrono::high_resolution_clock::now();
					RenderShapesInstancedShadow(ShadowType::DIRECTION, registry, visibleEntities);
					auto end = std::chrono::high_resolution_clock::now();
					//std::cout << "RenderPointLightShadowShapesInstanced = " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) << std::endl;
				}

				{
					auto start = std::chrono::high_resolution_clock::now();
					RenderModelShadow(ShadowType::DIRECTION, registry, visibleEntities);
					auto end = std::chrono::high_resolution_clock::now();
					//std::cout << "RenderPointLightShadowModel = " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) << std::endl;
				}

				{
					auto start = std::chrono::high_resolution_clock::now();
					RenderModelInstancedShadow(ShadowType::DIRECTION, registry, visibleEntities);
					auto end = std::chrono::high_resolution_clock::now();
					//std::cout << "RenderPointLightShadowModelInstanced = " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) << std::endl;
				}

				{
					auto start = std::chrono::high_resolution_clock::now();
					RenderAnimationShadow(ShadowType::DIRECTION, registry, visibleEntities);
					auto end = std::chrono::high_resolution_clock::now();
					//std::cout << "RenderPointLightShadowModelInstanced = " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) << std::endl;
				}

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

			if (pointLightComponent.toRender && pointLightComponent.useShadow)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, pointLightComponent.shadowFramebuffer);
				glViewport(0, 0, pointLightComponent.shadowSize, pointLightComponent.shadowSize);
				glClear(GL_DEPTH_BUFFER_BIT);
				resourceManager->GetSsbo("PointLightData")->BindBufferBase(0);
				resourceManager->GetSsbo("TransformData")->BindBufferBase(1);
				auto program = resourceManager->GetProgram("ShadowPoint");
				program->Bind();
				program->SetUniform("u_lightIndex", pointLightIndex);

				{
					auto start = std::chrono::high_resolution_clock::now();
					RenderShapesShadow(ShadowType::POINT,  registry, pointLightComponent.visibleEntities);
					auto end = std::chrono::high_resolution_clock::now();
					//std::cout << "RenderPointLightShadowShapes = " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) << std::endl;
				}

				{
					auto start = std::chrono::high_resolution_clock::now();
					RenderShapesInstancedShadow(ShadowType::POINT, registry, pointLightComponent.visibleEntities);
					auto end = std::chrono::high_resolution_clock::now();
					//std::cout << "RenderPointLightShadowShapesInstanced = " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) << std::endl;
				}

				{
					auto start = std::chrono::high_resolution_clock::now();
					RenderModelShadow(ShadowType::POINT, registry, pointLightComponent.visibleEntities);
					auto end = std::chrono::high_resolution_clock::now();
					//std::cout << "RenderPointLightShadowModel = " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) << std::endl;
				}

				{
					auto start = std::chrono::high_resolution_clock::now();
					RenderModelInstancedShadow(ShadowType::POINT, registry, pointLightComponent.visibleEntities);
					auto end = std::chrono::high_resolution_clock::now();
					//std::cout << "RenderPointLightShadowModelInstanced = " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) << std::endl;
				}

				{
					auto start = std::chrono::high_resolution_clock::now();
					RenderAnimationShadow(ShadowType::POINT, registry, pointLightComponent.visibleEntities);
					auto end = std::chrono::high_resolution_clock::now();
					//std::cout << "RenderPointLightShadowModelInstanced = " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) << std::endl;
				}

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

			if (spotLightComponent.toRender && spotLightComponent.useShadow)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, spotLightComponent.shadowFramebuffer);
				glViewport(0, 0, spotLightComponent.shadowSize, spotLightComponent.shadowSize);
				glClear(GL_DEPTH_BUFFER_BIT);
				resourceManager->GetSsbo("SpotLightData")->BindBufferBase(0);
				resourceManager->GetSsbo("TransformData")->BindBufferBase(1);
				auto program = resourceManager->GetProgram("ShadowSpot");
				program->Bind();
				program->SetUniform("u_lightIndex", spotLightIndex);

				{
					auto start = std::chrono::high_resolution_clock::now();
					RenderShapesShadow(ShadowType::SPOT, registry, spotLightComponent.visibleEntities);
					auto end = std::chrono::high_resolution_clock::now();
					//std::cout << "RenderPointLightShadowShapes = " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) << std::endl;
				}

				{
					auto start = std::chrono::high_resolution_clock::now();
					RenderShapesInstancedShadow(ShadowType::SPOT, registry, spotLightComponent.visibleEntities);
					auto end = std::chrono::high_resolution_clock::now();
					//std::cout << "RenderPointLightShadowShapesInstanced = " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) << std::endl;
				}

				{
					auto start = std::chrono::high_resolution_clock::now();
					RenderModelShadow(ShadowType::SPOT, registry, spotLightComponent.visibleEntities);
					auto end = std::chrono::high_resolution_clock::now();
					//std::cout << "RenderPointLightShadowModel = " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) << std::endl;
				}

				{
					auto start = std::chrono::high_resolution_clock::now();
					RenderModelInstancedShadow(ShadowType::SPOT, registry, spotLightComponent.visibleEntities);
					auto end = std::chrono::high_resolution_clock::now();
					//std::cout << "RenderPointLightShadowModelInstanced = " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) << std::endl;
				}

				{
					auto start = std::chrono::high_resolution_clock::now();
					RenderAnimationShadow(ShadowType::SPOT, registry, spotLightComponent.visibleEntities);
					auto end = std::chrono::high_resolution_clock::now();
					//std::cout << "RenderPointLightShadowModelInstanced = " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) << std::endl;
				}

				program->UnBind();
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}
	);
}

void ShadowRenderer::RenderShapesShadow(ShadowType type, std::shared_ptr<Registry> registry, std::vector<char>& visibleEntities)
{
	auto resourceManager = ResourceManager::Instance();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();

	if (!transformPool || !shapePool)
		return;

	auto program = resourceManager->GetProgram(type == ShadowType::DIRECTION ? "ShadowDir" : type == ShadowType::POINT ? "ShadowPoint" : "ShadowSpot");
	program->SetUniform("u_renderMode", (GLuint)0);

	std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (visibleEntities[entity] && transformPool->HasComponent(entity))
			{
				auto& shapeComponent = shapePool->GetComponent(entity);

				if (shapeComponent.shape && shapeComponent.castShadow && !shapeComponent.isInstanced)
				{
					auto transformIndex = transformPool->GetIndex(entity);
					program->SetUniform("u_transformIndex", transformIndex);
					shapeComponent.shape->Bind();
					glDrawElements(GL_TRIANGLES, shapeComponent.shape->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
					shapeComponent.shape->UnBind();
				}
			}
		}
	);
}

void ShadowRenderer::RenderShapesInstancedShadow(ShadowType type, std::shared_ptr<Registry> registry, std::vector<char>& visibleEntities)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	if (!transformPool || !shapePool)
		return;

	auto program = resourceManager->GetProgram(type == ShadowType::DIRECTION ? "ShadowDir" : type == ShadowType::POINT ? "ShadowPoint" : "ShadowSpot");
	program->SetUniform("u_renderMode", (GLuint)1);

	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;
		geometry->ClearShadowInstances();
	}

	std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (visibleEntities[entity] && transformPool->HasComponent(entity))
			{
				auto& shapeComponent = shapePool->GetComponent(entity);
				if (shapeComponent.shape && shapeComponent.isInstanced && shapeComponent.castShadow)
				{
					auto transformIndex = transformPool->GetIndex(entity);
					shapeComponent.shape->AddShadowInstanceID(transformIndex);
				}
			}
		}
	);

	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;
		geometry->UpdateShadowInstanceSsbo();
	}

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

void ShadowRenderer::RenderModelShadow(ShadowType type, std::shared_ptr<Registry> registry, std::vector<char>& visibleEntities)
{
	auto resourceManager = ResourceManager::Instance();
	auto animationPool = registry->GetComponentPool<AnimationComponent>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();
	if (!transformPool || !modelPool)
		return;

	auto program = resourceManager->GetProgram(type == ShadowType::DIRECTION ? "ShadowDir" : type == ShadowType::POINT ? "ShadowPoint" : "ShadowSpot");
	program->SetUniform("u_renderMode", (GLuint)2);

	std::for_each(std::execution::seq, modelPool->GetDenseEntitiesArray().begin(), modelPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (visibleEntities[entity] && transformPool->HasComponent(entity))
			{
				auto& modelComponent = modelPool->GetComponent(entity);

				if (modelComponent.model && !modelComponent.isInstanced && modelComponent.castShadow && (!animationPool || !animationPool->HasComponent(entity)))
				{
					auto transformIndex = transformPool->GetIndex(entity);
					program->SetUniform("u_transformIndex", transformIndex);
					modelComponent.model->Bind();
					glDrawElements(GL_TRIANGLES, modelComponent.model->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
					modelComponent.model->UnBind();
				}
			}
		}
	);
}

void ShadowRenderer::RenderModelInstancedShadow(ShadowType type, std::shared_ptr<Registry> registry, std::vector<char>& visibleEntities)
{
	auto resourceManager = ResourceManager::Instance();
	auto modelManager = ModelManager::Instance();
	auto animationPool = registry->GetComponentPool<AnimationComponent>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();

	if (!transformPool || !modelPool)
		return;

	auto program = resourceManager->GetProgram(type == ShadowType::DIRECTION ? "ShadowDir" : type == ShadowType::POINT ? "ShadowPoint" : "ShadowSpot");
	program->SetUniform("u_renderMode", (GLuint)3);

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;
		model->ClearShadowInstances();
	}

	std::for_each(std::execution::seq, modelPool->GetDenseEntitiesArray().begin(), modelPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (visibleEntities[entity] && transformPool->HasComponent(entity))
			{
				auto& modelComponent = modelPool->GetComponent(entity);

				if (modelComponent.model && modelComponent.isInstanced && modelComponent.castShadow && (!animationPool || !animationPool->HasComponent(entity)))
				{
					auto transformIndex = transformPool->GetIndex(entity);
					modelComponent.model->AddShadowInstanceID(transformIndex);
				}
			}
		}
	);

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;
		model->UpdateShadowInstanceSsbo();
	}

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


void ShadowRenderer::RenderAnimationShadow(ShadowType type, std::shared_ptr<Registry> registry, std::vector<char>& visibleEntities)
{
	auto resourceManager = ResourceManager::Instance();
	auto animationPool = registry->GetComponentPool<AnimationComponent>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();
	if (!transformPool || !modelPool || !animationPool)
		return;

	auto program = resourceManager->GetProgram(type == ShadowType::DIRECTION ? "ShadowDir" : type == ShadowType::POINT ? "ShadowPoint" : "ShadowSpot");
	program->SetUniform("u_renderMode", (GLuint)4);

	std::for_each(std::execution::seq, animationPool->GetDenseEntitiesArray().begin(), animationPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (visibleEntities[entity] && transformPool->HasComponent(entity) && modelPool->HasComponent(entity))
			{
				auto& modelComponent = modelPool->GetComponent(entity);
				auto& animationComponent = animationPool->GetComponent(entity);

				if (animationComponent.animation && modelComponent.model && modelComponent.castShadow)
				{
					animationComponent.boneTransformSsbo->BindBufferBase(3);
					animationComponent.animation->GetVertexBoneSsbo()->BindBufferBase(4);

					auto transformIndex = transformPool->GetIndex(entity);
					program->SetUniform("u_transformIndex", transformIndex);
					modelComponent.model->Bind();
					glDrawElements(GL_TRIANGLES, modelComponent.model->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
					modelComponent.model->UnBind();
				}
			}
		}
	);
}






















void ShadowRenderer::RenderShapesShadow(ShadowType type, std::shared_ptr<Registry> registry, std::vector<Entity>& visibleEntities)
{
	auto resourceManager = ResourceManager::Instance();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();

	if (!transformPool || !shapePool)
		return;

	auto program = resourceManager->GetProgram(type == ShadowType::DIRECTION ? "ShadowDir" : type == ShadowType::POINT ? "ShadowPoint" : "ShadowSpot");
	program->SetUniform("u_renderMode", (GLuint)0);

	std::for_each(std::execution::seq, visibleEntities.begin(), visibleEntities.end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) && shapePool->HasComponent(entity))
			{
				auto& shapeComponent = shapePool->GetComponent(entity);

				if (shapeComponent.shape && shapeComponent.castShadow && !shapeComponent.isInstanced)
				{
					auto transformIndex = transformPool->GetIndex(entity);
					program->SetUniform("u_transformIndex", transformIndex);
					shapeComponent.shape->Bind();
					glDrawElements(GL_TRIANGLES, shapeComponent.shape->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
					shapeComponent.shape->UnBind();
				}
			}
		}
	);
}

void ShadowRenderer::RenderShapesInstancedShadow(ShadowType type, std::shared_ptr<Registry> registry, std::vector<Entity>& visibleEntities)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	if (!transformPool || !shapePool)
		return;

	auto program = resourceManager->GetProgram(type == ShadowType::DIRECTION ? "ShadowDir" : type == ShadowType::POINT ? "ShadowPoint" : "ShadowSpot");
	program->SetUniform("u_renderMode", (GLuint)1);

	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;
		geometry->ClearShadowInstances();
	}

	std::for_each(std::execution::seq, visibleEntities.begin(), visibleEntities.end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) && shapePool->HasComponent(entity))
			{
				auto& shapeComponent = shapePool->GetComponent(entity);
				if (shapeComponent.shape && shapeComponent.isInstanced && shapeComponent.castShadow)
				{
					auto transformIndex = transformPool->GetIndex(entity);
					shapeComponent.shape->AddShadowInstanceID(transformIndex);
				}
			}
		}
	);

	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;
		geometry->UpdateShadowInstanceSsbo();
	}

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

void ShadowRenderer::RenderModelShadow(ShadowType type, std::shared_ptr<Registry> registry, std::vector<Entity>& visibleEntities)
{
	auto resourceManager = ResourceManager::Instance();
	auto animationPool = registry->GetComponentPool<AnimationComponent>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();
	if (!transformPool || !modelPool)
		return;

	auto program = resourceManager->GetProgram(type == ShadowType::DIRECTION ? "ShadowDir" : type == ShadowType::POINT ? "ShadowPoint" : "ShadowSpot");
	program->SetUniform("u_renderMode", (GLuint)2);

	std::for_each(std::execution::seq, visibleEntities.begin(), visibleEntities.end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) && modelPool->HasComponent(entity))
			{
				auto& modelComponent = modelPool->GetComponent(entity);

				if (modelComponent.model && !modelComponent.isInstanced && modelComponent.castShadow && (!animationPool || !animationPool->HasComponent(entity)))
				{
					auto transformIndex = transformPool->GetIndex(entity);
					program->SetUniform("u_transformIndex", transformIndex);
					modelComponent.model->Bind();
					glDrawElements(GL_TRIANGLES, modelComponent.model->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
					modelComponent.model->UnBind();
				}
			}
		}
	);
}

void ShadowRenderer::RenderModelInstancedShadow(ShadowType type, std::shared_ptr<Registry> registry, std::vector<Entity>& visibleEntities)
{
	auto resourceManager = ResourceManager::Instance();
	auto modelManager = ModelManager::Instance();
	auto animationPool = registry->GetComponentPool<AnimationComponent>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();

	if (!transformPool || !modelPool)
		return;

	auto program = resourceManager->GetProgram(type == ShadowType::DIRECTION ? "ShadowDir" : type == ShadowType::POINT ? "ShadowPoint" : "ShadowSpot");
	program->SetUniform("u_renderMode", (GLuint)3);

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;
		model->ClearShadowInstances();
	}

	std::for_each(std::execution::seq, visibleEntities.begin(), visibleEntities.end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) && modelPool->HasComponent(entity))
			{
				auto& modelComponent = modelPool->GetComponent(entity);

				if (modelComponent.model && modelComponent.isInstanced && modelComponent.castShadow && (!animationPool || !animationPool->HasComponent(entity)))
				{
					auto transformIndex = transformPool->GetIndex(entity);
					modelComponent.model->AddShadowInstanceID(transformIndex);
				}
			}
		}
	);

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;
		model->UpdateShadowInstanceSsbo();
	}

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
