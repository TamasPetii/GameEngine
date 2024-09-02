#include "SpotLightSystem.h"

void SpotLightSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void SpotLightSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto spotLightPool = registry->GetComponentPool<SpotLightComponent>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();

	if (!transformPool || !spotLightPool)
		return;

	static SpotLightGLSL* slDataSsboHandler = nullptr;
	static glm::mat4* slTransformSsboHandler = nullptr;
	static glm::vec4* slBillboardSsboHandler = nullptr;

	if (!slDataSsboHandler)
	{
		auto slDataSsbo = resourceManager->GetSsbo("SpotLightData");
		slDataSsboHandler = static_cast<SpotLightGLSL*>(slDataSsbo->MapBufferRange());
	}

	if (!slTransformSsboHandler)
	{
		auto slTransformSsbo = resourceManager->GetSsbo("SpotLightTransform");
		slTransformSsboHandler = static_cast<glm::mat4*>(slTransformSsbo->MapBufferRange());
	}

	if (!slBillboardSsboHandler)
	{
		auto slBillboardSsbo = resourceManager->GetSsbo("SpotLightBillboard");
		slBillboardSsboHandler = static_cast<glm::vec4*>(slBillboardSsbo->MapBufferRange());
	}

	//Dir,Spot,Point tranfsorm helyes használat ifben
	//Scale ne legyen hatással a pozícióra | mátrix szétbontás?
	std::for_each(std::execution::par, spotLightPool->GetDenseEntitiesArray().begin(), spotLightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (true || spotLightPool->IsFlagSet(entity, UPDATE_FLAG) || (transformPool->HasComponent(entity) && transformPool->IsFlagSet(entity, CHANGED_FLAG)))
			{
				auto& spotLightComponent = spotLightPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				auto index = spotLightPool->GetIndex(entity);

				spotLightComponent.position = transformComponent.translate;
				spotLightComponent.direction = glm::normalize(glm::vec3(transformComponent.modelTransform * glm::vec4(-1.f, -1.f, -1.f, 0.0f)));

				float scaleY = 0.5 * spotLightComponent.length;
				float scaleXZ = glm::tan(glm::radians(spotLightComponent.angles.y)) * scaleY * 2;
				glm::mat4 proxyTransform = glm::inverse(glm::lookAt<float>(spotLightComponent.position, spotLightComponent.position + glm::normalize(spotLightComponent.direction), glm::vec3(0.f, 1.f, 0.f)))
										 * glm::rotate<float>(glm::radians(90.f), glm::vec3(1, 0, 0))
										 * glm::scale(glm::vec3(scaleXZ, scaleY, scaleXZ))
										 * glm::translate(glm::vec3(0, -1, 0));

				//Recalculating viewProj matrix for shadow depth
				glm::mat4 view = glm::lookAt(spotLightComponent.position, spotLightComponent.position + spotLightComponent.direction, glm::vec3(0.f, 1.f, 0.f));
				glm::mat4 proj = glm::perspective(glm::radians(60.f), 1.f, 0.1f, spotLightComponent.length);
				spotLightComponent.farPlane = spotLightComponent.length;
				spotLightComponent.viewProj = proj * view;

				slDataSsboHandler[index].color = glm::vec4(spotLightComponent.color, spotLightComponent.strength);
				slDataSsboHandler[index].position = glm::vec4(spotLightComponent.position, spotLightComponent.useShadow ? 1 : 0);
				slDataSsboHandler[index].direction = glm::vec4(spotLightComponent.direction, spotLightComponent.farPlane);
				slDataSsboHandler[index].viewProj = spotLightComponent.viewProj;
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
				
				float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				glCreateTextures(GL_TEXTURE_2D, 1, &spotLightComponent.shadowTexture);
				glTextureParameteri(spotLightComponent.shadowTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTextureParameteri(spotLightComponent.shadowTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTextureParameteri(spotLightComponent.shadowTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(spotLightComponent.shadowTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTextureParameterfv(spotLightComponent.shadowTexture, GL_TEXTURE_BORDER_COLOR, borderColor);
				glTextureStorage2D(spotLightComponent.shadowTexture, 1, GL_DEPTH_COMPONENT24, spotLightComponent.shadowSize, spotLightComponent.shadowSize);
				glNamedFramebufferTexture(spotLightComponent.shadowFramebuffer, GL_DEPTH_ATTACHMENT, spotLightComponent.shadowTexture, 0);

				glNamedFramebufferDrawBuffer(spotLightComponent.shadowFramebuffer, GL_NONE);
				glNamedFramebufferReadBuffer(spotLightComponent.shadowFramebuffer, GL_NONE);

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

	/*
	slDataSsbo->UnMapBuffer();
	slTransformSsbo->UnMapBuffer();
	slBillboardSsbo->UnMapBuffer();
	*/
}

nlohmann::json SpotLightSystem::Serialize(Registry* registry, Entity entity)
{
	auto& spotLightComponent = registry->GetComponent<SpotLightComponent>(entity);

	nlohmann::json data;
	data["color"]["x"] = spotLightComponent.color.x;
	data["color"]["y"] = spotLightComponent.color.y;
	data["color"]["z"] = spotLightComponent.color.z;
	data["position"]["x"] = spotLightComponent.position.x;
	data["position"]["y"] = spotLightComponent.position.y;
	data["position"]["z"] = spotLightComponent.position.z;
	data["direction"]["x"] = spotLightComponent.direction.x;
	data["direction"]["y"] = spotLightComponent.direction.y;
	data["direction"]["z"] = spotLightComponent.direction.z;
	data["angles"]["x"] = spotLightComponent.angles.x;
	data["angles"]["y"] = spotLightComponent.angles.y;
	data["length"] = spotLightComponent.length;
	data["strength"] = spotLightComponent.strength;
	data["updateFrequency"] = spotLightComponent.updateFrequency;
	data["shadowSize"] = spotLightComponent.shadowSize;
	data["useShadow"] = spotLightComponent.useShadow;

	return data;
}

void SpotLightSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& spotLightComponent = registry->GetComponent<SpotLightComponent>(entity);

	spotLightComponent.color = glm::vec3(data["color"]["x"], data["color"]["y"], data["color"]["z"]);
	spotLightComponent.position = glm::vec3(data["position"]["x"], data["position"]["y"], data["position"]["z"]);
	spotLightComponent.direction = glm::vec3(data["direction"]["x"], data["direction"]["y"], data["direction"]["z"]);
	spotLightComponent.angles = glm::vec2(data["angles"]["x"], data["angles"]["y"]);
	spotLightComponent.length = static_cast<float>(data["length"]);
	spotLightComponent.strength = static_cast<float>(data["strength"]);
	spotLightComponent.updateFrequency = static_cast<int>(data["updateFrequency"]);
	spotLightComponent.shadowSize = static_cast<int>(data["shadowSize"]);
	spotLightComponent.useShadow = static_cast<bool>(data["useShadow"]);

	registry->SetFlag<SpotLightComponent>(entity, UPDATE_FLAG);
	registry->SetFlag<SpotLightComponent>(entity, REGENERATE_FLAG);
}