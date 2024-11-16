#include "GeometryRenderer.h"

void GeometryRenderer::Render(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->Bind();
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6 });

	auto program = resourceManager->GetProgram("DeferredPre");
	program->Bind();
	RenderShapes(registry);
	RenderShapesInstanced(registry);
	RenderModel(registry);
	RenderModelInstanced(registry);
	RenderModelAnimated(registry);
	program->UnBind();

	fbo->UnBind();
}

void GeometryRenderer::RenderShapes(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();
	auto materialPool = registry->GetComponentPool<MaterialComponent>();

	if (!transformPool || !materialPool || !shapePool )
		return;

	auto fbo = resourceManager->GetFbo("Main");
	auto program = resourceManager->GetProgram("DeferredPre");
	program->SetUniform("u_renderMode", (GLuint)0);
	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("TransformData")->BindBufferBase(1);
	resourceManager->GetSsbo("MaterialData")->BindBufferBase(3);
	resourceManager->GetSsbo("ShapeData")->BindBufferBase(4);

	std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) && materialPool->HasComponent(entity))
			{
				auto& shapeComponent = shapePool->GetComponent(entity);

				if (shapeComponent.shape && shapeComponent.toRender && !shapeComponent.isInstanced)
				{
					auto transformIndex = transformPool->GetIndex(entity);
					auto materialIndex = materialPool->GetIndex(entity);
					auto shapeIndex = shapePool->GetIndex(entity);
					auto entityIndex = entity;

					program->SetUniform("u_shapeModelIndex", shapeIndex);
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

void GeometryRenderer::RenderShapesInstanced(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	auto program = resourceManager->GetProgram("DeferredPre");
	program->SetUniform("u_renderMode", (GLuint)1);

	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("TransformData")->BindBufferBase(1);
	resourceManager->GetSsbo("MaterialData")->BindBufferBase(3);
	resourceManager->GetSsbo("ShapeData")->BindBufferBase(4);

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

void GeometryRenderer::RenderModel(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();
	auto animationPool = registry->GetComponentPool<AnimationComponent>();

	if (!transformPool || !modelPool)
		return;

	auto fbo = resourceManager->GetFbo("Main");
	auto program = resourceManager->GetProgram("DeferredPre");
	program->SetUniform("u_renderMode", (GLuint)2);

	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("TransformData")->BindBufferBase(1);
	resourceManager->GetSsbo("ModelData")->BindBufferBase(4);

	std::for_each(std::execution::seq, modelPool->GetDenseEntitiesArray().begin(), modelPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity))
			{
				auto& modelComponent = modelPool->GetComponent(entity);
				
				if (modelComponent.model && modelComponent.toRender && !modelComponent.isInstanced && (!animationPool || (!animationPool->HasComponent(entity) || animationPool->GetComponent(entity).animation == nullptr)))
				{
					auto transformIndex = transformPool->GetIndex(entity);
					auto modelIndex = modelPool->GetIndex(entity);
					auto entityIndex = entity;

					program->SetUniform("u_shapeModelIndex", modelIndex);
					program->SetUniform("u_transformIndex", transformIndex);
					program->SetUniform("u_entityIndex", entityIndex);
					modelComponent.model->GetMaterialSsbo()->BindBufferBase(3);
					modelComponent.model->Bind();
					glDrawElements(GL_TRIANGLES, modelComponent.model->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
					modelComponent.model->UnBind();
				}
			}
		}
	);
}

void GeometryRenderer::RenderModelInstanced(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto modelManager = ModelManager::Instance();

	auto fbo = resourceManager->GetFbo("Main");
	auto program = resourceManager->GetProgram("DeferredPre");
	program->SetUniform("u_renderMode", (GLuint)3);

	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("TransformData")->BindBufferBase(1);
	resourceManager->GetSsbo("ModelData")->BindBufferBase(4);

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;

		if (model && model->GetInstances().size() > 0)
		{
			model->GetInstanceSsbo()->BindBufferBase(2);
			model->GetMaterialSsbo()->BindBufferBase(3);

			model->Bind();
			glDrawElementsInstanced(GL_TRIANGLES, model->GetIndexCount(), GL_UNSIGNED_INT, nullptr, model->GetInstances().size());
			model->UnBind();
		}
	}
}

void checkGLError() {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cerr << "OpenGL Error: " << err << std::endl;
	}
}

void GeometryRenderer::RenderModelAnimated(std::shared_ptr<Registry> registry)
{
	auto modelPool = registry->GetComponentPool<ModelComponent>();
	auto animationPool = registry->GetComponentPool<AnimationComponent>();

	if (!modelPool || !animationPool)
		return;

	auto resourceManager = ResourceManager::Instance();
	auto program = resourceManager->GetProgram("DeferredPre");
	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("TransformData")->BindBufferBase(1);
	resourceManager->GetSsbo("ModelData")->BindBufferBase(4);
	program->SetUniform("u_renderMode", (GLuint)4);

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
				auto modelIndex = registry->GetIndex<ModelComponent>(entity);

				if (modelComponent.toRender && !modelComponent.isInstanced)
				{
					modelComponent.model->GetMaterialSsbo()->BindBufferBase(3);
					animationComponent.boneTransformSsbo->BindBufferBase(5);
					animationComponent.animation->GetVertexBoneSsbo()->BindBufferBase(6);

					program->SetUniform("u_shapeModelIndex", modelIndex);
					program->SetUniform("u_transformIndex", transformIndex);
					program->SetUniform("u_entityIndex", entityIndex);
					modelComponent.model->Bind();
					glDrawElements(GL_TRIANGLES, modelComponent.model->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
					modelComponent.model->UnBind();
				}
			}
		}
	);
}