#include "SpotLightSystem.h"

#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/Light/SpotLightComponent.h"
#include "Registry/Component/TransformComponent.h"

#include "Render/OpenGL/FramebufferGL.h"
#include "Render/OpenGL/ShaderStorageBufferGL.h"
#include "Settings/GlobalSettings.h"

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

	auto slDataSsbo = resourceManager->GetSsbo("SpotLightData");
	if (slDataSsbo->GetBufferHandler() == nullptr)
		slDataSsbo->MapBufferRange();
	SpotLightGLSL* slDataSsboHandler = static_cast<SpotLightGLSL*>(slDataSsbo->GetBufferHandler());

	auto slTransformSsbo = resourceManager->GetSsbo("SpotLightTransform");
	if (slTransformSsbo->GetBufferHandler() == nullptr)
		slTransformSsbo->MapBufferRange();
	glm::mat4* slTransformSsboHandler = static_cast<glm::mat4*>(slTransformSsbo->GetBufferHandler());

	auto slBillboardSsbo = resourceManager->GetSsbo("SpotLightBillboard");
	if (slBillboardSsbo->GetBufferHandler() == nullptr)
		slBillboardSsbo->MapBufferRange();
	glm::vec4* slBillboardSsboHandler = static_cast<glm::vec4*>(slBillboardSsbo->GetBufferHandler());

	if (!slDataSsboHandler || !slTransformSsboHandler || !slBillboardSsboHandler || spotLightPool->GetSize() > resourceManager->GetComponentSsboSize<SpotLightComponent>())
		return;

	std::for_each(std::execution::par, spotLightPool->GetDenseEntitiesArray().begin(), spotLightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) && (spotLightPool->IsFlagSet(entity, UPDATE_FLAG) || transformPool->IsFlagSet(entity, CHANGED_FLAG)))
			{
				auto& spotLightComponent = spotLightPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				auto index = spotLightPool->GetIndex(entity);

				spotLightComponent.position = transformComponent.translate;
				spotLightComponent.direction = glm::normalize(glm::vec3(transformComponent.modelTransform * glm::vec4(-1.f, -1.f, -1.f, 0.0f)));

				spotLightComponent.angles.z = cosf(glm::radians(spotLightComponent.angles.x));
				spotLightComponent.angles.w = cosf(glm::radians(spotLightComponent.angles.y));

				float scaleY = 0.5 * spotLightComponent.length;
				float scaleXZ = glm::tan(glm::radians(spotLightComponent.angles.y)) * scaleY * 2;
				spotLightComponent.proxyTransform = glm::inverse(glm::lookAt<float>(spotLightComponent.position,
																 spotLightComponent.position + glm::normalize(spotLightComponent.direction),
					                                             glm::vec3(0.f, 1.f, 0.f)))
												  * glm::rotate<float>(glm::radians(90.f), glm::vec3(1, 0, 0))
												  * glm::scale(glm::vec3(scaleXZ, scaleY, scaleXZ))
												  * glm::translate(glm::vec3(0, -1, 0));

				glm::mat4 view = glm::lookAt(spotLightComponent.position, spotLightComponent.position + spotLightComponent.direction, glm::vec3(0.f, 1.f, 0.f));
				glm::mat4 proj = glm::perspective(glm::radians(spotLightComponent.angles.y * 2), 1.f, 0.01f, spotLightComponent.length);
				spotLightComponent.farPlane = spotLightComponent.length;
				spotLightComponent.viewProj = proj * view;

				slDataSsboHandler[index].color = glm::vec4(spotLightComponent.color, spotLightComponent.strength);
				slDataSsboHandler[index].position = glm::vec4(spotLightComponent.position, spotLightComponent.useShadow ? 1 : 0);
				slDataSsboHandler[index].direction = glm::vec4(spotLightComponent.direction, spotLightComponent.farPlane);
				slDataSsboHandler[index].viewProj = spotLightComponent.viewProj;
				slDataSsboHandler[index].angles = spotLightComponent.angles;
				slTransformSsboHandler[index] = spotLightComponent.proxyTransform;

				if (!GlobalSettings::GameViewActive)
				{
					slBillboardSsboHandler[index] = glm::vec4(spotLightComponent.position, entity);
					slTransformSsboHandler[index] = spotLightComponent.proxyTransform;
				}

				spotLightPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);

	std::for_each(std::execution::seq, spotLightPool->GetDenseEntitiesArray().begin(), spotLightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (spotLightPool->IsFlagSet(entity, REGENERATE_FLAG))
			{
				auto& spotLightComponent = spotLightPool->GetComponent(entity);
				auto index = spotLightPool->GetIndex(entity);

				if (spotLightComponent.useShadow)
				{
					constexpr auto spotLightParamTextureFunction = [](unsigned int textureID) -> void {
						float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
						glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
						glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
						glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
						glTextureParameterfv(textureID, GL_TEXTURE_BORDER_COLOR, borderColor);
						};


					TextureSpecGL depthTextureSpec;
					depthTextureSpec.attachment = GL_DEPTH_ATTACHMENT;
					depthTextureSpec.textureType = GL_TEXTURE_2D;
					depthTextureSpec.internalFormat = GL_DEPTH_COMPONENT24;
					depthTextureSpec.type = GL_FLOAT;
					depthTextureSpec.generateHandler = true;
					depthTextureSpec.generateMipMap = false;
					depthTextureSpec.paramTextureFunction = spotLightParamTextureFunction;

					spotLightComponent.frameBuffer = std::make_shared<FramebufferGL>(spotLightComponent.shadowSize, spotLightComponent.shadowSize);
					spotLightComponent.frameBuffer->AttachTexture("depth", depthTextureSpec);
					spotLightComponent.frameBuffer->CheckCompleteness();
					slDataSsboHandler[index].shadowTexture = spotLightComponent.frameBuffer->GetTextureHandler("depth");
					spotLightPool->ResFlag(entity, REGENERATE_FLAG);
				}
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
	spotLightComponent.angles.x = data["angles"]["x"];
	spotLightComponent.angles.y = data["angles"]["y"];
	spotLightComponent.length = static_cast<float>(data["length"]);
	spotLightComponent.strength = static_cast<float>(data["strength"]);
	spotLightComponent.updateFrequency = static_cast<int>(data["updateFrequency"]);
	spotLightComponent.shadowSize = static_cast<int>(data["shadowSize"]);
	spotLightComponent.useShadow = static_cast<bool>(data["useShadow"]);

	registry->SetFlag<SpotLightComponent>(entity, UPDATE_FLAG);
	registry->SetFlag<SpotLightComponent>(entity, REGENERATE_FLAG);
}