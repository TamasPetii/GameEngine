#include "GeometryRenderer.h"

void GeometryRenderer::Render(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->Bind();
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT5 });

	RenderStaticObjects(registry);

	RenderModelAnimated(registry);
}

void GeometryRenderer::RenderStaticObjects(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto program = resourceManager->GetProgram("DeferredPre");
	program->Bind();
	RenderShapes(registry);
	RenderShapesInstanced(registry);
	RenderModel(registry);
	RenderModelInstanced(registry);
	program->UnBind();
}

void GeometryRenderer::RenderDynamicObjects(std::shared_ptr<Registry> registry)
{

}

void GeometryRenderer::RenderShapes(std::shared_ptr<Registry> registry)
{
	auto shapePool = registry->GetComponentPool<ShapeComponent>();
	if (!shapePool)
		return;

	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	auto program = resourceManager->GetProgram("DeferredPre");
	program->SetUniform("u_renderMode", (GLuint)0);

	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("TransformData")->BindBufferBase(1);
	resourceManager->GetSsbo("MaterialData")->BindBufferBase(3);
	resourceManager->GetSsbo("ShapeData")->BindBufferBase(4);

	std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (registry->HasComponent<TransformComponent>(entity) && registry->HasComponent<MaterialComponent>(entity))
			{
				auto& shapeComponent = shapePool->GetComponent(entity);
				auto transformIndex = registry->GetIndex<TransformComponent>(entity);
				auto materialIndex = registry->GetIndex<MaterialComponent>(entity);
				auto shapeIndex = registry->GetIndex<ShapeComponent>(entity);
				auto entityIndex = entity;

				if (shapeComponent.toRender && !shapeComponent.isInstanced)
				{
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

	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
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
	auto modelPool = registry->GetComponentPool<ModelComponent>();
	if (!modelPool)
		return;

	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	auto program = resourceManager->GetProgram("DeferredPre");
	program->SetUniform("u_renderMode", (GLuint)2);

	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("TransformData")->BindBufferBase(1);
	resourceManager->GetSsbo("ModelData")->BindBufferBase(4);

	std::for_each(std::execution::seq, modelPool->GetDenseEntitiesArray().begin(), modelPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (registry->HasComponent<TransformComponent>(entity) && !registry->HasComponent<AnimationComponent>(entity))
			{
				auto& modelComponent = modelPool->GetComponent(entity);
				auto transformIndex = registry->GetIndex<TransformComponent>(entity);
				auto modelIndex = registry->GetIndex<ModelComponent>(entity);
				auto entityIndex = entity;

				if (modelComponent.model && modelComponent.toRender && !modelComponent.isInstanced)
				{
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

	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("TransformData")->BindBufferBase(1);
	resourceManager->GetSsbo("ModelData")->BindBufferBase(4);

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;
		if (model->GetInstanceCount() > 0)
		{
			model->GetInstanceSsbo()->BindBufferBase(2);
			model->GetMaterialSsbo()->BindBufferBase(3);

			model->Bind();
			glDrawElementsInstanced(GL_TRIANGLES, model->GetIndexCount(), GL_UNSIGNED_INT, nullptr, model->GetInstanceCount());
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