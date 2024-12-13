#include "WaterRenderer.h"
#include "Render/SkyboxRenderer.h"

#include <algorithm>
#include <execution>

#include <glm/gtx/transform2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Registry/Registry.h"
#include "Manager/ModelManager.h"
#include "Manager/ResourceManager.h"
#include "Manager/TextureManager.h"
#include "Settings/GlobalSettings.h"

#include "Render/Geometry/Geometry.h"
#include "Render/OpenGL/ProgramGL.h"
#include "Render/OpenGL/FramebufferGL.h"
#include "Render/OpenGL/ShaderStorageBufferGL.h"

#include "Model/Model.h"
#include "Animation/Animation.h"
#include "Registry/Component/Object/ShapeComponent.h"
#include "Registry/Component/Object/ModelComponent.h"
#include "Registry/Component/Object/AnimationComponent.h"
#include "Registry/Component/Object/MaterialComponent.h"
#include "Registry/Component/TransformComponent.h"
#include "Registry/Component/WaterComponent.h"
#include "Registry/Component/CameraComponent.h"
#include "Registry/System/CameraSystem.h"

void WaterRenderer::Render(std::shared_ptr<Registry> registry)
{
	RenderPreWater(registry);
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
	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);
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

void WaterRenderer::RenderPreWater(std::shared_ptr<Registry> registry)
{
	static int64_t frameCount = 0;
	frameCount++;

	auto& cameraComponent = CameraSystem::GetMainCamera(registry);

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

			if(frameCount % waterComponent.updateFrequency == 0)
			{
				//0 = relfection | 1 = refraction
				for (int i = 0; i <= 1; i++)
				{
					auto fbo = i == REFLECTION ? waterComponent.reflectionFbo : waterComponent.refractionFbo;

					static float distance;
					if (i == REFLECTION)
					{
						distance = 2 * (cameraComponent.position.y - waterComponent.plane.w);
						cameraComponent.position.y -= distance;
						CameraSystem::InvertPitch(cameraComponent);
						CameraSystem::UpdateMatrices(cameraComponent);
						CameraSystem::UpdateToGpu(cameraComponent);
					}

					fbo->Clear();
					fbo->Bind();
					fbo->ActivateTexture(GL_COLOR_ATTACHMENT0);

					auto program = resourceManager->GetProgram("WaterPre");
					program->Bind();
					program->SetUniform("u_waterIndex", waterIndex);
					program->SetUniform("u_reflection", (unsigned int)i);
					resourceManager->GetSsbo("CameraData")->BindBufferBase(0);
					resourceManager->GetSsbo("TransformData")->BindBufferBase(1);
					resourceManager->GetSsbo("WaterData")->BindBufferBase(2);

					RenderShapes(registry);
					RenderShapesInstanced(registry);
					RenderModel(registry);
					RenderModelInstanced(registry);
					RenderAnimation(registry);
					RenderSkybox(registry);

					program->UnBind();
					fbo->UnBind();

					if (i == REFLECTION)
					{
						cameraComponent.position.y += distance;
						CameraSystem::InvertPitch(cameraComponent);
						CameraSystem::UpdateMatrices(cameraComponent);
						CameraSystem::UpdateToGpu(cameraComponent);
					}

					fbo->UnBind();
				}
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
	program->SetUniform("u_renderMode", (unsigned int)0);

	std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (registry->HasComponent<TransformComponent>(entity) && registry->HasComponent<MaterialComponent>(entity))
			{
				auto& shapeComponent = shapePool->GetComponent(entity);
				auto transformIndex = registry->GetIndex<TransformComponent>(entity);
				auto materialIndex = registry->GetIndex<MaterialComponent>(entity);
				auto entityIndex = entity;

				if (shapeComponent.shape && !shapeComponent.isInstanced)
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
	program->SetUniform("u_renderMode", (unsigned int)1);

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
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();

	if (!transformPool || !modelPool)
		return;

	auto program = resourceManager->GetProgram("WaterPre");
	program->SetUniform("u_renderMode", (unsigned int)2);

	std::for_each(std::execution::seq, modelPool->GetDenseEntitiesArray().begin(), modelPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (registry->HasComponent<TransformComponent>(entity) && !registry->HasComponent<AnimationComponent>(entity))
			{
				auto& modelComponent = modelPool->GetComponent(entity);
				auto transformIndex = registry->GetIndex<TransformComponent>(entity);
				auto entityIndex = entity;

				if (modelComponent.model && !modelComponent.isInstanced)
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
	program->SetUniform("u_renderMode", (unsigned int)3);

	for (auto& data : modelManager->GetModelsList())
	{
		auto model = data.second;
		if (model && model->GetInstances().size() > 0)
		{
			model->GetInstanceSsbo()->BindBufferBase(3);
			model->GetMaterialSsbo()->BindBufferBase(4);

			model->Bind();
			glDrawElementsInstanced(GL_TRIANGLES, model->GetIndexCount(), GL_UNSIGNED_INT, nullptr, model->GetInstances().size());
			model->UnBind();
		}
	}
}

void WaterRenderer::RenderSkybox(std::shared_ptr<Registry> registry)
{
	if (!SkyboxRenderer::UseSkybox || !SkyboxRenderer::SkyboxTexture)
		return;

	auto& cameraComponent = CameraSystem::GetMainCamera(registry);

	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);
	glDepthFunc(GL_LEQUAL);

	auto textureManager = TextureManager::Instance();
	auto resourceManager = ResourceManager::Instance();

	auto program = resourceManager->GetProgram("WaterPre");
	program->SetUniform("u_renderMode", (unsigned int)4);
	program->SetUniform("u_skyboxModel", glm::translate(cameraComponent.position) * glm::scale(glm::vec3(-1.f)));
	program->SetTexture("u_skyboxTexture", 0, SkyboxRenderer::SkyboxTexture->GetTextureID());

	resourceManager->GetGeometry("Cube")->Bind();
	glDrawElements(GL_TRIANGLES, resourceManager->GetGeometry("Cube")->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	resourceManager->GetGeometry("Cube")->UnBind();

	glDepthFunc(prevDepthFnc);
}

void WaterRenderer::RenderAnimation(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();
	auto animationPool = registry->GetComponentPool<AnimationComponent>();

	if (!transformPool || !modelPool || !animationPool)
		return;

	auto program = resourceManager->GetProgram("WaterPre");
	program->SetUniform("u_renderMode", (unsigned int)5);

	std::for_each(std::execution::seq, animationPool->GetDenseEntitiesArray().begin(), animationPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) && modelPool->HasComponent(entity))
			{
				auto& animationComponent = animationPool->GetComponent(entity);
				auto& modelComponent = modelPool->GetComponent(entity);
				auto transformIndex = transformPool->GetIndex(entity);
				auto entityIndex = entity;

				if (animationComponent.animation && modelComponent.model)
				{
					program->SetUniform("u_transformIndex", transformIndex);
					program->SetUniform("u_entityIndex", entityIndex);
					modelComponent.model->GetMaterialSsbo()->BindBufferBase(4);
					animationComponent.boneTransformSsbo->BindBufferBase(5);
					animationComponent.animation->GetVertexBoneSsbo()->BindBufferBase(6);
					modelComponent.model->Bind();
					glDrawElements(GL_TRIANGLES, modelComponent.model->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
					modelComponent.model->UnBind();
				}
			}
		}
	);
}