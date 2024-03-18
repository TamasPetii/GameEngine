#include "PointLightSystem.h"

void PointLightSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void PointLightSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto pointLightPool = registry->GetComponentPool<PointLightComponent>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();

	if (!transformPool || !pointLightPool)
		return;

	static bool init = true;
	static PointLightGLSL* plDataSsboHandler = nullptr;
	static glm::mat4* plTransformSsboHandler = nullptr;
	static glm::vec4* plBillboardSsboHandler = nullptr;
	if (init)
	{
		init = false;
		auto plDataSsbo = resourceManager->GetSsbo("PointLightData");
		plDataSsboHandler = static_cast<PointLightGLSL*>(plDataSsbo->MapBuffer(GL_WRITE_ONLY));
		auto plTransformSsbo = resourceManager->GetSsbo("PointLightTransform");
		plTransformSsboHandler = static_cast<glm::mat4*>(plTransformSsbo->MapBuffer(GL_WRITE_ONLY));
		auto plBillboardSsbo = resourceManager->GetSsbo("PointLightBillboard");
		plBillboardSsboHandler = static_cast<glm::vec4*>(plBillboardSsbo->MapBuffer(GL_WRITE_ONLY));
	}

	std::for_each(std::execution::par, pointLightPool->GetDenseEntitiesArray().begin(), pointLightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (true || pointLightPool->IsFlagSet(entity, UPDATE_FLAG))
			{
				auto& pointLightComponent = pointLightPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				auto index = pointLightPool->GetIndex(entity);

				float farPlane = glm::sqrt(256.f * pointLightComponent.strength / 5.f);
				glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.f, 0.1f, pointLightComponent.farPlane);
				pointLightComponent.farPlane = farPlane;
				pointLightComponent.viewProj[0] = proj * glm::lookAt(pointLightComponent.position, pointLightComponent.position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
				pointLightComponent.viewProj[1] = proj * glm::lookAt(pointLightComponent.position, pointLightComponent.position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
				pointLightComponent.viewProj[2] = proj * glm::lookAt(pointLightComponent.position, pointLightComponent.position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
				pointLightComponent.viewProj[3] = proj * glm::lookAt(pointLightComponent.position, pointLightComponent.position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
				pointLightComponent.viewProj[4] = proj * glm::lookAt(pointLightComponent.position, pointLightComponent.position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
				pointLightComponent.viewProj[5] = proj * glm::lookAt(pointLightComponent.position, pointLightComponent.position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));

				pointLightComponent.position = transformComponent.translate;
				plDataSsboHandler[index].color = glm::vec4(pointLightComponent.color, pointLightComponent.strength);
				plDataSsboHandler[index].position = glm::vec4(pointLightComponent.position, pointLightComponent.useShadow ? 1 : 0);
				plDataSsboHandler[index].farPlane = glm::vec2(pointLightComponent.farPlane, 0);
				plDataSsboHandler[index].viewProj[0] = pointLightComponent.viewProj[0];
				plDataSsboHandler[index].viewProj[1] = pointLightComponent.viewProj[1];
				plDataSsboHandler[index].viewProj[2] = pointLightComponent.viewProj[2];
				plDataSsboHandler[index].viewProj[3] = pointLightComponent.viewProj[3];
				plDataSsboHandler[index].viewProj[4] = pointLightComponent.viewProj[4];
				plDataSsboHandler[index].viewProj[5] = pointLightComponent.viewProj[5];

				plBillboardSsboHandler[index] = glm::vec4(pointLightComponent.position, entity);
				plTransformSsboHandler[index] = glm::translate(pointLightComponent.position) * glm::scale(glm::vec3(pointLightComponent.farPlane));

				pointLightPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);

	std::for_each(std::execution::seq, pointLightPool->GetDenseEntitiesArray().begin(), pointLightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			auto& pointLightComponent = pointLightPool->GetComponent(entity);
			auto index = pointLightPool->GetIndex(entity);

			if (pointLightComponent.useShadow && pointLightPool->IsFlagSet(entity, REGENERATE_FLAG))
			{
				glMakeTextureHandleNonResidentARB(pointLightComponent.shadowTextureHandler);
				glDeleteFramebuffers(1, &pointLightComponent.shadowFramebuffer);
				glDeleteTextures(1, &pointLightComponent.shadowTexture);

				glCreateFramebuffers(1, &pointLightComponent.shadowFramebuffer);
				glBindFramebuffer(GL_FRAMEBUFFER, pointLightComponent.shadowFramebuffer);


				float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &pointLightComponent.shadowTexture);
				glTextureParameteri(pointLightComponent.shadowTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTextureParameteri(pointLightComponent.shadowTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTextureParameteri(pointLightComponent.shadowTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(pointLightComponent.shadowTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTextureParameteri(pointLightComponent.shadowTexture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTextureParameterfv(pointLightComponent.shadowTexture, GL_TEXTURE_BORDER_COLOR, borderColor);
				glTextureStorage2D(pointLightComponent.shadowTexture, 1, GL_DEPTH_COMPONENT24, pointLightComponent.shadowSize, pointLightComponent.shadowSize);
				glNamedFramebufferTexture(pointLightComponent.shadowFramebuffer, GL_DEPTH_ATTACHMENT, pointLightComponent.shadowTexture, 0);
				
				if (glCheckNamedFramebufferStatus(pointLightComponent.shadowFramebuffer, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
					throw std::runtime_error("Error occurred while creating frame buffer!");

				pointLightComponent.shadowTextureHandler = glGetTextureHandleARB(pointLightComponent.shadowTexture);
				glMakeTextureHandleResidentARB(pointLightComponent.shadowTextureHandler);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				plDataSsboHandler[index].shadowTexture = pointLightComponent.shadowTextureHandler;
				pointLightPool->ResFlag(entity, REGENERATE_FLAG);
			}
		}
	);

	//plDataSsbo->UnMapBuffer();
	//plTransformSsbo->UnMapBuffer();
	//plBillboardSsbo->UnMapBuffer();
}
