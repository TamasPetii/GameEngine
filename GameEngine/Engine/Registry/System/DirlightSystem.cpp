#include "DirlightSystem.h"

void DirlightSystem::OnStart(std::shared_ptr<Registry> registry, std::shared_ptr<Camera> camera)
{
}

void DirlightSystem::OnUpdate(std::shared_ptr<Registry> registry, std::shared_ptr<Camera> camera)
{
	auto resourceManager = ResourceManager::Instance();
	auto dirlightPool = registry->GetComponentPool<DirlightComponent>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();

	if (!dirlightPool || !transformPool)
		return;

	static bool init = true;
	static DirlightGLSL* dlDataSsboHandler = nullptr;
	static glm::vec4* dlBillboardSsboHandler = nullptr;
	static DirlightLineGLSL* dlLinesSsboHandler = nullptr;

	if (init)
	{
		init = false;
		auto dlDataSsbo = resourceManager->GetSsbo("DirLightData");
		dlDataSsboHandler = static_cast<DirlightGLSL*>(dlDataSsbo->MapBuffer(GL_WRITE_ONLY));
		auto dlBillboardSsbo = resourceManager->GetSsbo("DirLightBillboard");
		dlBillboardSsboHandler = static_cast<glm::vec4*>(dlBillboardSsbo->MapBuffer(GL_WRITE_ONLY));
		auto dlLinesSsbo = resourceManager->GetSsbo("DirLightLines");
		dlLinesSsboHandler = static_cast<DirlightLineGLSL*>(dlLinesSsbo->MapBuffer(GL_WRITE_ONLY));
	}

	std::for_each(std::execution::par, dirlightPool->GetDenseEntitiesArray().begin(), dirlightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			//TODO ONLY IF CAMERA CHANGES
			if (true || dirlightPool->IsFlagSet(entity, UPDATE_FLAG) || (transformPool->HasComponent(entity) && transformPool->IsFlagSet(entity, CHANGED_FLAG)))
			{
				auto& dirlightComponent = dirlightPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				auto index = dirlightPool->GetIndex(entity);

				dirlightComponent.direction = glm::normalize(glm::vec3(transformComponent.modelTransform * glm::vec4(-1.f, -1.f, -1.f, 0.0f)));

				if (dirlightComponent.useShadow)
				{
					for (int i = 0; i < 4; i++)
					{
						glm::mat4 cameraProj = glm::perspective(glm::radians(camera->GetFOV()), camera->GetWidth() / camera->GetHeight(), dirlightComponent.farPlane[i], dirlightComponent.farPlane[i + 1]);
						glm::mat4 cameraView = camera->GetView();
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

						glm::mat4 shadowView = glm::lookAt<float>(center - glm::normalize(dirlightComponent.direction), center, glm::vec3(0.0f, 1.0f, 0.0f));

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

						float zMult = 10.f;
						if (minZ < 0)
						{
							minZ *= zMult;
						}
						else
						{
							minZ /= zMult;
						}
						if (maxZ < 0)
						{
							maxZ /= zMult;
						}
						else
						{
							maxZ *= zMult;
						}

						glm::mat4 shadowProj = glm::ortho<float>(minX, maxX, minY, maxY, minZ, maxZ);
						glm::mat4 shadowViewProj = shadowProj * shadowView;
						dirlightComponent.viewProj[i] = shadowViewProj;
					}

					dlDataSsboHandler[index].farPlane = glm::vec4(dirlightComponent.farPlane[1], dirlightComponent.farPlane[2], dirlightComponent.farPlane[3], dirlightComponent.farPlane[4]);
					dlDataSsboHandler[index].viewProj[0] = dirlightComponent.viewProj[0];
					dlDataSsboHandler[index].viewProj[1] = dirlightComponent.viewProj[1];
					dlDataSsboHandler[index].viewProj[2] = dirlightComponent.viewProj[2];
					dlDataSsboHandler[index].viewProj[3] = dirlightComponent.viewProj[3];
				}
				
				dlDataSsboHandler[index].color = glm::vec4(dirlightComponent.color, dirlightComponent.strength);
				dlDataSsboHandler[index].direction = glm::vec4(dirlightComponent.direction, dirlightComponent.useShadow ? 1 : 0);
				
				dlLinesSsboHandler[index].position = glm::vec4(transformComponent.translate, 1);
				dlLinesSsboHandler[index].direction = glm::vec4( dirlightComponent.direction, 1);

				dlBillboardSsboHandler[index] = glm::vec4(transformComponent.translate, entity);

				dirlightPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);

	std::for_each(std::execution::seq, dirlightPool->GetDenseEntitiesArray().begin(), dirlightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			auto& dirlightComponent = dirlightPool->GetComponent(entity);
			auto index = dirlightPool->GetIndex(entity);

			if(dirlightComponent.useShadow && dirlightPool->IsFlagSet(entity, REGENERATE_FLAG))
			{
				glMakeTextureHandleNonResidentARB(dirlightComponent.shadowTextureHandler);
				glDeleteFramebuffers(1, &dirlightComponent.shadowFramebuffer);
				glDeleteTextures(1, &dirlightComponent.shadowTexture);
				//glDeleteTextures(4, dirlightComponent.shadowTextureView);

				glCreateFramebuffers(1, &dirlightComponent.shadowFramebuffer);
				glBindFramebuffer(GL_FRAMEBUFFER, dirlightComponent.shadowFramebuffer);

				float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &dirlightComponent.shadowTexture);
				glTextureParameteri(dirlightComponent.shadowTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTextureParameteri(dirlightComponent.shadowTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTextureParameteri(dirlightComponent.shadowTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(dirlightComponent.shadowTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTextureParameterfv(dirlightComponent.shadowTexture, GL_TEXTURE_BORDER_COLOR, borderColor);
				glTextureStorage3D(dirlightComponent.shadowTexture, 1, GL_DEPTH_COMPONENT24, dirlightComponent.shadowSize, dirlightComponent.shadowSize, 4);
				glNamedFramebufferTexture(dirlightComponent.shadowFramebuffer, GL_DEPTH_ATTACHMENT, dirlightComponent.shadowTexture, 0);

				glNamedFramebufferDrawBuffer(dirlightComponent.shadowFramebuffer, GL_NONE);
				glNamedFramebufferReadBuffer(dirlightComponent.shadowFramebuffer, GL_NONE);

				if (glCheckNamedFramebufferStatus(dirlightComponent.shadowFramebuffer, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
					throw std::runtime_error("Error occurred while creating frame buffer!");

				/*
				glCreateTextures(GL_TEXTURE_2D, 4, dirlightComponent.shadowTextureView);
				glTextureView(dirlightComponent.shadowTextureView[0], GL_TEXTURE_2D, dirlightComponent.shadowTexture, GL_DEPTH_COMPONENT32, 0, 1, 0, 1);
				glTextureView(dirlightComponent.shadowTextureView[1], GL_TEXTURE_2D, dirlightComponent.shadowTexture, GL_DEPTH_COMPONENT32, 0, 1, 1, 1);
				glTextureView(dirlightComponent.shadowTextureView[2], GL_TEXTURE_2D, dirlightComponent.shadowTexture, GL_DEPTH_COMPONENT32, 0, 1, 2, 1);
				glTextureView(dirlightComponent.shadowTextureView[3], GL_TEXTURE_2D, dirlightComponent.shadowTexture, GL_DEPTH_COMPONENT32, 0, 1, 3, 1);
				*/

				dirlightComponent.shadowTextureHandler = glGetTextureHandleARB(dirlightComponent.shadowTexture);
				glMakeTextureHandleResidentARB(dirlightComponent.shadowTextureHandler);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				dlDataSsboHandler[index].shadowTexture = dirlightComponent.shadowTextureHandler;
				dirlightPool->ResFlag(entity, REGENERATE_FLAG);
			}
		}
	);

	//dlBillboardSsbo->UnMapBuffer();
	//dlDataSsbo->UnMapBuffer();
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