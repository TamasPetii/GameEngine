#include "DirlightSystem.h"

#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/Light/DirLightcomponent.h"
#include "Registry/Component/TransformComponent.h"
#include "Registry/Component/CameraComponent.h"
#include "Registry/System/CameraSystem.h"

#include "Render/OpenGL/FramebufferGL.h"
#include "Render/OpenGL/ShaderStorageBufferGL.h"
#include "Render/WireframeRenderer.h"
#include "Settings/GlobalSettings.h"

void DirlightSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void DirlightSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto dirlightPool = registry->GetComponentPool<DirlightComponent>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto cameraPool = registry->GetComponentPool<CameraComponent>();

	if (!dirlightPool || !transformPool || !cameraPool)
		return;

	auto dlDataSsbo = resourceManager->GetSsbo("DirLightData");
	if (dlDataSsbo->GetBufferHandler() == nullptr)
		dlDataSsbo->MapBufferRange();
	DirlightGLSL* dlDataSsboHandler = static_cast<DirlightGLSL*>(dlDataSsbo->GetBufferHandler());

	auto dlBillboardSsbo = resourceManager->GetSsbo("DirLightBillboard");
	if (dlBillboardSsbo->GetBufferHandler() == nullptr)
		dlBillboardSsbo->MapBufferRange();
	glm::vec4* dlBillboardSsboHandler = static_cast<glm::vec4*>(dlBillboardSsbo->GetBufferHandler());

	auto dlLinesSsbo = resourceManager->GetSsbo("DirLightLines");
	if (dlLinesSsbo->GetBufferHandler() == nullptr)
		dlLinesSsbo->MapBufferRange();
	DirlightLineGLSL* dlLinesSsboHandler = static_cast<DirlightLineGLSL*>(dlLinesSsbo->GetBufferHandler());

	if (!dlDataSsboHandler || !dlBillboardSsboHandler || !dlLinesSsboHandler || dirlightPool->GetSize() > resourceManager->GetComponentSsboSize<DirlightComponent>())
		return;

	auto& cameraComponent = CameraSystem::GetMainCamera(registry);

	std::for_each(std::execution::par, dirlightPool->GetDenseEntitiesArray().begin(), dirlightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (true || dirlightPool->IsFlagSet(entity, UPDATE_FLAG) || ((transformPool->HasComponent(entity) && transformPool->IsFlagSet(entity, CHANGED_FLAG))))
			{
				auto& dirlightComponent = dirlightPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				auto index = dirlightPool->GetIndex(entity);

				dirlightComponent.direction = glm::normalize(glm::vec3(transformComponent.modelTransform * glm::vec4(-1.f, -1.f, -1.f, 0.0f)));

				if (dirlightComponent.useShadow)
				{
					for (int i = 0; i < 4; i++)
					{
						glm::mat4 cameraProj = glm::perspective(glm::radians(cameraComponent.fov), cameraComponent.width / cameraComponent.height, dirlightComponent.farPlane[i], dirlightComponent.farPlane[i + 1]);
						glm::mat4 cameraView = cameraComponent.view;
						glm::mat4 viewProjInv = glm::inverse(cameraProj * cameraView);

						std::vector<glm::vec4> frustumCorners;
						for (int x = 0; x < 2; ++x)
						{
							for (int y = 0; y < 2; ++y)
							{
								for (int z = 0; z < 2; ++z)
								{
									float x_ncd = 2.0f * x - 1.0f;
									float y_ncd = 2.0f * y - 1.0f;
									float z_ncd = 2.0f * z - 1.0f;
									glm::vec4 pt = viewProjInv * glm::vec4(x_ncd, y_ncd, z_ncd, 1.0f);
									pt /= pt.w;
									frustumCorners.push_back(pt);
								}
							}
						}

						glm::vec3 center = glm::vec3(0, 0, 0);
						for (auto& v : frustumCorners)
							center += glm::vec3(v.x, v.y, v.z);
						center /= frustumCorners.size();

						glm::mat4 shadowView = glm::lookAt<float>(center, center + glm::normalize(dirlightComponent.direction), glm::vec3(0.0f, 1.0f, 0.0f));

						float minX = std::numeric_limits<float>::max();
						float maxX = std::numeric_limits<float>::lowest();
						float minY = std::numeric_limits<float>::max();
						float maxY = std::numeric_limits<float>::lowest();
						float minZ = std::numeric_limits<float>::max();
						float maxZ = std::numeric_limits<float>::lowest();

						for (auto& v : frustumCorners)
						{
							auto trf = shadowView * v;
							minX = std::min(minX, trf.x);
							maxX = std::max(maxX, trf.x);
							minY = std::min(minY, trf.y);
							maxY = std::max(maxY, trf.y);
							minZ = std::min(minZ, trf.z);
							maxZ = std::max(maxZ, trf.z);
						}

						//glm::mat4 shadowProj = glm::ortho<float>(minX, maxX, minY, maxY, minZ, maxZ);
						glm::mat4 shadowProj = glm::ortho<float>(minX, maxX, minY, maxY, -500, 500);
						glm::mat4 shadowViewProj = shadowProj * shadowView;
						dirlightComponent.viewProj[i] = shadowViewProj;
						dlDataSsboHandler[index].viewProj[i] = dirlightComponent.viewProj[i];
					}

					dlDataSsboHandler[index].farPlane = glm::vec4(dirlightComponent.farPlane[1], dirlightComponent.farPlane[2], dirlightComponent.farPlane[3], dirlightComponent.farPlane[4]);
				}

				dlDataSsboHandler[index].color = glm::vec4(dirlightComponent.color, dirlightComponent.strength);
				dlDataSsboHandler[index].direction = glm::vec4(dirlightComponent.direction, dirlightComponent.useShadow ? 1 : 0);			
				
				//Update dirlight icon billboard data
				if (!GlobalSettings::GameViewActive)
				{
					dlBillboardSsboHandler[index] = glm::vec4(transformComponent.translate, entity);
				}

				//Update dirlight direction line data
				if (!GlobalSettings::GameViewActive && WireframeRenderer::ShowDirLightsLines)
				{
					dlLinesSsboHandler[index].position = glm::vec4(transformComponent.translate, 1);
					dlLinesSsboHandler[index].direction = glm::vec4(dirlightComponent.direction, 1);
				}

				dirlightPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);

	std::for_each(std::execution::seq, dirlightPool->GetDenseEntitiesArray().begin(), dirlightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (dirlightPool->IsFlagSet(entity, REGENERATE_FLAG))
			{
				auto& dirlightComponent = dirlightPool->GetComponent(entity);
				auto index = dirlightPool->GetIndex(entity);

				if (dirlightComponent.useShadow)
				{
					constexpr auto dirLightParamTextureFunction = [](unsigned int textureID) -> void {
						float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
						glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
						glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
						glTextureParameterfv(textureID, GL_TEXTURE_BORDER_COLOR, borderColor);
						};


					TextureSpecGL depthTextureSpec;
					depthTextureSpec.is2D = false;
					depthTextureSpec.layer = 4;
					depthTextureSpec.attachment = GL_DEPTH_ATTACHMENT;
					depthTextureSpec.textureType = GL_TEXTURE_2D_ARRAY;
					depthTextureSpec.internalFormat = GL_DEPTH_COMPONENT32F;
					depthTextureSpec.type = GL_FLOAT;
					depthTextureSpec.generateHandler = true;
					depthTextureSpec.generateMipMap = false;
					depthTextureSpec.paramTextureFunction = dirLightParamTextureFunction;

					dirlightComponent.frameBuffer = std::make_shared<FramebufferGL>(dirlightComponent.shadowSize, dirlightComponent.shadowSize);
					dirlightComponent.frameBuffer->AttachTexture("depth", depthTextureSpec);
					dirlightComponent.frameBuffer->CheckCompleteness();

					dlDataSsboHandler[index].shadowTexture = dirlightComponent.frameBuffer->GetTextureHandler("depth");
					dirlightPool->ResFlag(entity, REGENERATE_FLAG);
				}
			}
		}
	);


	/*
	dlDataSsbo->UnMapBuffer();
	dlBillboardSsbo->UnMapBuffer();
	dlLinesSsbo->UnMapBuffer();
	*/
}

nlohmann::json DirlightSystem::Serialize(Registry* registry, Entity entity)
{
	auto& dirLightComponent = registry->GetComponent<DirlightComponent>(entity);

	nlohmann::json data;
	data["color"]["x"] = dirLightComponent.color.x;
	data["color"]["y"] = dirLightComponent.color.y;
	data["color"]["z"] = dirLightComponent.color.z;
	data["direction"]["x"] = dirLightComponent.direction.x;
	data["direction"]["y"] = dirLightComponent.direction.y;
	data["direction"]["z"] = dirLightComponent.direction.z;
	data["strength"] = dirLightComponent.strength;
	data["updateFrequency"] = dirLightComponent.updateFrequency;
	data["shadowSize"] = dirLightComponent.shadowSize;
	data["useShadow"] = dirLightComponent.useShadow;

	return data;
}

void DirlightSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& dirLightComponent = registry->GetComponent<DirlightComponent>(entity);

	dirLightComponent.color = glm::vec3(data["color"]["x"], data["color"]["y"], data["color"]["z"]);
	dirLightComponent.direction = glm::vec3(data["direction"]["x"], data["direction"]["y"], data["direction"]["z"]);
	dirLightComponent.strength = data["strength"];
	dirLightComponent.updateFrequency = data["updateFrequency"];
	dirLightComponent.shadowSize = data["shadowSize"];
	dirLightComponent.useShadow = data["useShadow"];

	registry->SetFlag<DirlightComponent>(entity, UPDATE_FLAG);
	registry->SetFlag<DirlightComponent>(entity, REGENERATE_FLAG);
}