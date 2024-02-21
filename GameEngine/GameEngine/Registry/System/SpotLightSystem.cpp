#include "SpotLightSystem.h"

void SpotLightSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void SpotLightSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto slDataSsbo = resourceManager->GetSsbo("SpotLightData");
	auto slDataSsboHandler = static_cast<SpotLightGLSL*>(slDataSsbo->MapBuffer(GL_WRITE_ONLY));
	auto slTransformSsbo = resourceManager->GetSsbo("SpotLightTransform");
	auto slTransformSsboHandler = static_cast<glm::mat4*>(slTransformSsbo->MapBuffer(GL_WRITE_ONLY));
	auto slBillboardSsbo = resourceManager->GetSsbo("SpotLightBillboard");
	auto slBillboardSsboHandler = static_cast<glm::vec4*>(slBillboardSsbo->MapBuffer(GL_WRITE_ONLY));
	auto spotLightPool = registry->GetComponentPool<SpotLightComponent>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();

	std::for_each(std::execution::par, spotLightPool->GetDenseEntitiesArray().begin(), spotLightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (spotLightPool->IsFlagSet(entity, UPDATE_FLAG))
			{
				auto& spotLightComponent = spotLightPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				auto index = spotLightPool->GetIndex(entity);

				spotLightComponent.position = transformComponent.translate;

				float scaleY = 0.5 * spotLightComponent.length;
				float scaleXZ = glm::tan(glm::radians(spotLightComponent.angles.y)) * scaleY;
				glm::mat4 proxyTransform = glm::inverse(glm::lookAt<float>(spotLightComponent.position, spotLightComponent.position + glm::normalize(spotLightComponent.direction), glm::vec3(0.f, 1.f, 0.f)))
										 * glm::rotate<float>(glm::radians(90.f), glm::vec3(1, 0, 0))
										 * glm::scale(glm::vec3(scaleXZ, scaleY, scaleXZ))
										 * glm::translate(glm::vec3(0, -1, 0));

				slDataSsboHandler[index].color = glm::vec4(spotLightComponent.color, spotLightComponent.strength);
				slDataSsboHandler[index].position = glm::vec4(spotLightComponent.position, spotLightComponent.useShadow ? 1 : 0);
				slDataSsboHandler[index].direction = glm::vec4(spotLightComponent.direction, spotLightComponent.farPlane);
				slDataSsboHandler[index].viewProj = glm::mat4(1);
				slDataSsboHandler[index].angles = glm::vec2(cosf(glm::radians(spotLightComponent.angles.x)), cosf(glm::radians(spotLightComponent.angles.y)));					
				slBillboardSsboHandler[index] = glm::vec4(spotLightComponent.position, entity);
				slTransformSsboHandler[index] = proxyTransform;

				spotLightPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);

	std::for_each(std::execution::seq, spotLightPool->GetDenseEntitiesArray().begin(), spotLightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			auto& spotLightComponent = spotLightPool->GetComponent(entity);
			auto index = spotLightPool->GetIndex(entity);

			if (spotLightComponent.useShadow && spotLightPool->IsFlagSet(entity, REGENERATE_FLAG))
			{
				glMakeTextureHandleNonResidentARB(spotLightComponent.shadowTextureHandler);
				glDeleteFramebuffers(1, &spotLightComponent.shadowFramebuffer);
				glDeleteTextures(1, &spotLightComponent.shadowTexture);

				glCreateFramebuffers(1, &spotLightComponent.shadowFramebuffer);
				glBindFramebuffer(GL_FRAMEBUFFER, spotLightComponent.shadowFramebuffer);

				glCreateTextures(GL_TEXTURE_2D, 1, &spotLightComponent.shadowTexture);
				glTextureParameteri(spotLightComponent.shadowTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTextureParameteri(spotLightComponent.shadowTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTextureParameteri(spotLightComponent.shadowTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(spotLightComponent.shadowTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTextureStorage2D(spotLightComponent.shadowTexture, 1, GL_DEPTH_COMPONENT24, spotLightComponent.shadowSize, spotLightComponent.shadowSize);
				glNamedFramebufferTexture(spotLightComponent.shadowFramebuffer, GL_DEPTH_ATTACHMENT, spotLightComponent.shadowTexture, 0);

				if (glCheckNamedFramebufferStatus(spotLightComponent.shadowFramebuffer, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
					throw std::runtime_error("Error occurred while creating frame buffer!");

				spotLightComponent.shadowTextureHandler = glGetTextureHandleARB(spotLightComponent.shadowTexture);
				glMakeTextureHandleResidentARB(spotLightComponent.shadowTextureHandler);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				slDataSsboHandler[index].shadowTexture = spotLightComponent.shadowTextureHandler;
				spotLightPool->ResFlag(entity, REGENERATE_FLAG);
			}
		}
	);

	slDataSsbo->UnMapBuffer();
	slTransformSsbo->UnMapBuffer();
	slBillboardSsbo->UnMapBuffer();
}
