#include "WaterRenderer.h"

void WaterRenderer::Render(std::shared_ptr<Registry> registry, std::shared_ptr<Camera> camera)
{
	RenderPreWater(registry, camera);
	RenderWater(registry);
}

void WaterRenderer::RenderWater(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto waterPool = registry->GetComponentPool<WaterComponent>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();

	if (!waterPool || !transformPool)
		return;

	auto fbo = resourceManager->GetFbo("Main");
	fbo->Bind();
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT3 });

	auto program = resourceManager->GetProgram("Water");
	program->Bind();
	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("TransformData")->BindBufferBase(1);
	resourceManager->GetSsbo("WaterData")->BindBufferBase(2);

	std::for_each(std::execution::seq, waterPool->GetDenseEntitiesArray().begin(), waterPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity))
			{
				auto& waterComponent = waterPool->GetComponent(entity);
				auto entityIndex = waterPool->GetIndex(entity);
				auto transformIndex = transformPool->GetIndex(entity);

				program->SetUniform("u_transformIndex", transformIndex);
				program->SetUniform("u_entityIndex", entityIndex);
				program->SetUniform("u_entityID", entity);
				program->SetTexture("u_reflection", 0, waterComponent.reflectionFbo->GetTextureID("reflection"));
				program->SetTexture("u_refraction", 1, waterComponent.refractionFbo->GetTextureID("refraction"));

				//Could be instanced
				auto cube = resourceManager->GetGeometry("Cube");
				cube->Bind();
				glDrawElements(GL_TRIANGLES, cube->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
				cube->UnBind();
			}		
		});

	program->UnBind();
	fbo->UnBind();
}

void WaterRenderer::RenderPreWater(std::shared_ptr<Registry> registry, std::shared_ptr<Camera> camera)
{
	const int REFLECTION = 0;
	const int REFRACTION = 1;

	glEnable(GL_CLIP_DISTANCE0);

	auto resourceManager = ResourceManager::Instance();
	auto waterPool = registry->GetComponentPool<WaterComponent>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();

	if (!waterPool || !transformPool)
		return;

	std::for_each(std::execution::seq, waterPool->GetDenseEntitiesArray().begin(), waterPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entityWater) -> void {
			auto& waterComponent = waterPool->GetComponent(entityWater);
			auto waterIndex = waterPool->GetIndex(entityWater);

			//0 = relfection | 1 = refraction
			for (int i = 0; i <= 1; i++)
			{
				auto fbo = i == REFLECTION ? waterComponent.reflectionFbo : waterComponent.refractionFbo;

				static float distance;
				if (i == REFLECTION)
				{
					distance = 2 * (camera->GetPosition().y - waterComponent.plane.w);
					camera->RefPosition().y -= distance;
					camera->InvertPitch();
					camera->UpdateView();
					camera->UpdateProj();
					camera->UpdateGLSL();
				}

				fbo->Clear();
				fbo->Bind();
				fbo->ActivateTexture(GL_COLOR_ATTACHMENT0);

				auto program = resourceManager->GetProgram("WaterPre");
				program->Bind();
				program->SetUniform("u_waterIndex", waterIndex);
				program->SetUniform("u_reflection", (GLuint)i);
				resourceManager->GetUbo("CameraData")->BindBufferBase(0);
				resourceManager->GetSsbo("TransformData")->BindBufferBase(1);
				resourceManager->GetSsbo("WaterData")->BindBufferBase(2);

				RenderShapes(registry);
				RenderShapesInstanced(registry);
				RenderModel(registry);
				RenderModelInstanced(registry);
				RenderSkybox(registry, camera);

				program->UnBind();
				fbo->UnBind();

				if (i == REFLECTION)
				{
					camera->RefPosition().y += distance;
					camera->InvertPitch();
					camera->UpdateView();
					camera->UpdateProj();
					camera->UpdateGLSL();
				}

				fbo->UnBind();
			}
		});

	glDisable(GL_CLIP_DISTANCE0);
}

void WaterRenderer::RenderShapes(std::shared_ptr<Registry> registry)
{
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	if (!shapePool)
		return;

	auto resourceManager = ResourceManager::Instance();
	resourceManager->GetSsbo("MaterialData")->BindBufferBase(4);

	auto program = resourceManager->GetProgram("WaterPre");
	program->SetUniform("u_renderMode", (GLuint)0);

	std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (registry->HasComponent<TransformComponent>(entity) && registry->HasComponent<MaterialComponent>(entity))
			{
				auto& shapeComponent = shapePool->GetComponent(entity);
				auto transformIndex = registry->GetIndex<TransformComponent>(entity);
				auto materialIndex = registry->GetIndex<MaterialComponent>(entity);
				auto entityIndex = entity;

				if (!shapeComponent.isInstanced)
				{
					program->SetUniform("u_transformIndex", transformIndex);
					program->SetUniform("u_materialIndex", materialIndex);
					program->SetUniform("u_entityIndex", entityIndex);
					shapeComponent.shape->Bind();
					glDrawElements(GL_TRIANGLES, shapeComponent.shape->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
					shapeComponent.shape->UnBind();
				}
			}
		}
	);
}

void WaterRenderer::RenderShapesInstanced(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	resourceManager->GetSsbo("MaterialData")->BindBufferBase(4);

	auto program = resourceManager->GetProgram("WaterPre");
	program->SetUniform("u_renderMode", (GLuint)1);

	for (auto& data : resourceManager->GetGeometryList())
	{
		auto geometry = data.second;

		if (geometry->GetInstances().size() > 0)
		{
			geometry->GetInstanceSsbo()->BindBufferBase(3);
			geometry->Bind();
			glDrawElementsInstanced(GL_TRIANGLES, geometry->GetIndexCount(), GL_UNSIGNED_INT, nullptr, geometry->GetInstances().size());
			geometry->UnBind();
		}
	}
}

void WaterRenderer::RenderModel(std::shared_ptr<Registry> registry)
{
	auto modelPool = registry->GetComponentPool<ModelComponent>();

	if (!modelPool)
		return;

	auto resourceManager = ResourceManager::Instance();
	auto program = resourceManager->GetProgram("WaterPre");
	program->SetUniform("u_renderMode", (GLuint)2);

	std::for_each(std::execution::seq, modelPool->GetDenseEntitiesArray().begin(), modelPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (registry->HasComponent<TransformComponent>(entity) && !registry->HasComponent<AnimationComponent>(entity))
			{
				auto& modelComponent = modelPool->GetComponent(entity);
				auto transformIndex = registry->GetIndex<TransformComponent>(entity);
				auto entityIndex = entity;

				if (!modelComponent.isInstanced)
				{
					program->SetUniform("u_transformIndex", transformIndex);
					program->SetUniform("u_entityIndex", entityIndex);
					modelComponent.model->GetMaterialSsbo()->BindBufferBase(4);
					modelComponent.model->Bind();
					glDrawElements(GL_TRIANGLES, modelComponent.model->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
					modelComponent.model->UnBind();
				}
			}
		}
	);
}

void WaterRenderer::RenderModelInstanced(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto modelManager = ModelManager::Instance();
	auto program = resourceManager->GetProgram("WaterPre");
	program->SetUniform("u_renderMode", (GLuint)3);

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;
		if (model->GetInstanceCount() > 0)
		{
			model->GetInstanceSsbo()->BindBufferBase(3);
			model->GetMaterialSsbo()->BindBufferBase(4);

			model->Bind();
			glDrawElementsInstanced(GL_TRIANGLES, model->GetIndexCount(), GL_UNSIGNED_INT, nullptr, model->GetInstanceCount());
			model->UnBind();
		}
	}
}

void WaterRenderer::RenderSkybox(std::shared_ptr<Registry> registry, std::shared_ptr<Camera> camera)
{
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);
	glDepthFunc(GL_LEQUAL);

	auto textureManager = TextureManager::Instance();
	auto resourceManager = ResourceManager::Instance();

	auto program = resourceManager->GetProgram("WaterPre");
	program->SetUniform("u_renderMode", (GLuint)4);
	program->SetUniform("u_skyboxModel", glm::translate(camera->GetPosition()) * glm::scale(glm::vec3(-1.f)));
	program->SetTexture("u_skyboxTexture", 0, textureManager->LoadImageTextureMap("../Assets/sky.png")->GetTextureID());

	resourceManager->GetGeometry("Cube")->Bind();
	glDrawElements(GL_TRIANGLES, resourceManager->GetGeometry("Cube")->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	resourceManager->GetGeometry("Cube")->UnBind();

	glDepthFunc(prevDepthFnc);
}

/*

void WaterRenderer::RenderModelAnimated(std::shared_ptr<Registry> registry)
{
	auto modelPool = registry->GetComponentPool<ModelComponent>();
	auto animationPool = registry->GetComponentPool<AnimationComponent>();

	if (!modelPool || !animationPool)
		return;

	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT5 });

	auto program = resourceManager->GetProgram("ModelAnimation");
	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("TransformData")->BindBufferBase(1);
	program->Bind();

	std::for_each(std::execution::seq, animationPool->GetDenseEntitiesArray().begin(), animationPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (registry->HasComponent<TransformComponent>(entity) &&
				registry->HasComponent<ModelComponent>(entity) &&
				registry->GetComponent<ModelComponent>(entity).model != nullptr &&
				registry->GetComponent<AnimationComponent>(entity).animation != nullptr)
			{
				auto& modelComponent = modelPool->GetComponent(entity);
				auto& animationComponent = animationPool->GetComponent(entity);
				auto& transformComponent = registry->GetComponent<TransformComponent>(entity);
				auto transformIndex = registry->GetIndex<TransformComponent>(entity);
				auto entityIndex = entity;

				if (modelComponent.toRender && !modelComponent.isInstanced)
				{
					animationComponent.boneTransformSsbo->BindBufferBase(2);
					animationComponent.animation->GetVertexBoneSsbo()->BindBufferBase(3);
					modelComponent.model->GetMaterialSsbo()->BindBufferBase(4);

					program->SetUniform("u_transformIndex", transformIndex);
					program->SetUniform("u_entityIndex", entityIndex);
					modelComponent.model->Bind();
					glDrawElements(GL_TRIANGLES, modelComponent.model->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
					modelComponent.model->UnBind();
				}
			}
		}
	);

	program->UnBind();
}
*/