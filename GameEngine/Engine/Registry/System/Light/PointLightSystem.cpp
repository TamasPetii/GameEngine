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

	static PointLightGLSL* plDataSsboHandler = nullptr;
	static glm::mat4* plTransformSsboHandler = nullptr;
	static glm::vec4* plBillboardSsboHandler = nullptr;

	auto plDataSsbo = resourceManager->GetSsbo("PointLightData");
	if (!plDataSsboHandler)
	{
		plDataSsboHandler = static_cast<PointLightGLSL*>(plDataSsbo->MapBufferRange());
	}

	auto plTransformSsbo = resourceManager->GetSsbo("PointLightTransform");
	if (!plTransformSsboHandler)
	{
		plTransformSsboHandler = static_cast<glm::mat4*>(plTransformSsbo->MapBufferRange());
	}

	auto plBillboardSsbo = resourceManager->GetSsbo("PointLightBillboard");
	if (!plBillboardSsboHandler)
	{
		plBillboardSsboHandler = static_cast<glm::vec4*>(plBillboardSsbo->MapBufferRange());
	}


	std::for_each(std::execution::seq, pointLightPool->GetDenseEntitiesArray().begin(), pointLightPool->GetDenseEntitiesArray().end(),
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
				constexpr auto pointLightParamTextureFunction = [](GLuint textureID) -> void {
					float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
					glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glTextureParameteri(textureID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
					glTextureParameterfv(textureID, GL_TEXTURE_BORDER_COLOR, borderColor);
					};


				TextureSpecGL depthTextureSpec;
				depthTextureSpec.attachment = GL_DEPTH_ATTACHMENT;
				depthTextureSpec.textureType = GL_TEXTURE_CUBE_MAP;
				depthTextureSpec.internalFormat = GL_DEPTH_COMPONENT24;
				depthTextureSpec.type = GL_FLOAT;
				depthTextureSpec.generateHandler = true;
				depthTextureSpec.generateMipMap = false;
				depthTextureSpec.paramTextureFunction = pointLightParamTextureFunction;

				pointLightComponent.frameBuffer = std::make_shared<FramebufferGL>(pointLightComponent.shadowSize, pointLightComponent.shadowSize);
				pointLightComponent.frameBuffer->AttachTexture("depth", depthTextureSpec);
				pointLightComponent.frameBuffer->CheckCompleteness();

				plDataSsboHandler[index].shadowTexture = pointLightComponent.frameBuffer->GetTextureHandler("depth");
				pointLightPool->ResFlag(entity, REGENERATE_FLAG);
			}
		}
	);

	/*
	plDataSsbo->UnMapBuffer();
	plTransformSsbo->UnMapBuffer();
	plBillboardSsbo->UnMapBuffer();
	plDataSsboHandler = nullptr;
	plTransformSsboHandler = nullptr;
	plBillboardSsboHandler = nullptr;
	*/
}

nlohmann::json PointLightSystem::Serialize(Registry* registry, Entity entity)
{
	auto& pointLightComponent = registry->GetComponent<PointLightComponent>(entity);

	nlohmann::json data;
	data["color"]["x"] = pointLightComponent.color.x;
	data["color"]["y"] = pointLightComponent.color.y;
	data["color"]["z"] = pointLightComponent.color.z;
	data["position"]["x"] = pointLightComponent.position.x;
	data["position"]["y"] = pointLightComponent.position.y;
	data["position"]["z"] = pointLightComponent.position.z;
	data["strength"] = pointLightComponent.strength;
	data["updateFrequency"] = pointLightComponent.updateFrequency;
	data["shadowSize"] = pointLightComponent.shadowSize;
	data["useShadow"] = pointLightComponent.useShadow;

	return data;
}

void PointLightSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& pointLightComponent = registry->GetComponent<PointLightComponent>(entity);

	pointLightComponent.color = glm::vec3(data["color"]["x"], data["color"]["y"], data["color"]["z"]);
	pointLightComponent.position = glm::vec3(data["position"]["x"], data["position"]["y"], data["position"]["z"]);
	pointLightComponent.strength = data["strength"];
	pointLightComponent.updateFrequency = data["updateFrequency"];
	pointLightComponent.shadowSize = data["shadowSize"];
	pointLightComponent.useShadow = data["useShadow"];

	registry->SetFlag<PointLightComponent>(entity, UPDATE_FLAG);
	registry->SetFlag<PointLightComponent>(entity, REGENERATE_FLAG);
}