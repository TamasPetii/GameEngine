#include "SkyboxRenderer.h"

bool SkyboxRenderer::UseSkybox = true;
glm::vec3 SkyboxRenderer::SkyboxRotation = glm::vec3(0.f);
glm::vec3 SkyboxRenderer::SkyboxRotationSpeed = glm::vec3(0.f);
glm::ivec3 SkyboxRenderer::SkyboxRotationDirection = glm::ivec3(0);
glm::mat4 SkyboxRenderer::SkyboxRotationMatrix = glm::mat4(1);
std::shared_ptr<TextureGL> SkyboxRenderer::SkyboxTexture = nullptr;

void SkyboxRenderer::Render(std::shared_ptr<Registry> registry)
{
	if (!UseSkybox || !SkyboxTexture)
		return;

	SkyboxRotationMatrix = glm::rotate(glm::radians(SkyboxRotation.z), glm::vec3(0, 0, 1))
											* glm::rotate(glm::radians(SkyboxRotation.y), glm::vec3(0, 1, 0))
											* glm::rotate(glm::radians(SkyboxRotation.x), glm::vec3(1, 0, 0));

	auto textureManager = TextureManager::Instance();
	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->Bind();
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4 });
	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);

	auto& cameraComponent = CameraSystem::GetMainCamera(registry);

	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);
	glDepthFunc(GL_LEQUAL);
	auto program = resourceManager->GetProgram("Skybox");
	program->Bind();
	program->SetUniform("model", glm::translate(glm::vec3(cameraComponent.position)) * SkyboxRotationMatrix * glm::scale(glm::vec3(-1.f)));
	program->SetTexture("skyboxTexture", 0, SkyboxTexture->GetTextureID());

	resourceManager->GetGeometry("Cube")->Bind();
	glDrawElements(GL_TRIANGLES, resourceManager->GetGeometry("Cube")->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();
	glDepthFunc(prevDepthFnc);

	fbo->UnBind();
}

nlohmann::json SkyboxRenderer::Serialize()
{
	auto textureManager = TextureManager::Instance();
	nlohmann::json data;

	data["useSkybox"] = UseSkybox;
	data["SkyboxRotationSpeed"]["x"] = SkyboxRotationSpeed.x;
	data["SkyboxRotationSpeed"]["y"] = SkyboxRotationSpeed.y;
	data["SkyboxRotationSpeed"]["z"] = SkyboxRotationSpeed.z;
	data["skyboxRotationDirection"]["x"] = SkyboxRotationDirection.x;
	data["skyboxRotationDirection"]["y"] = SkyboxRotationDirection.y;
	data["skyboxRotationDirection"]["z"] = SkyboxRotationDirection.z;

	std::string skyboxPath = "none";
	if (SkyboxTexture)
	{
		if (SkyboxTexture->GetPath().substr(0, GlobalSettings::ProjectPath.size()) == GlobalSettings::ProjectPath)
			skyboxPath = SkyboxTexture->GetPath().substr(GlobalSettings::ProjectPath.size() + 1); //The +1 for deleting / from the start of the path
		else
			skyboxPath = SkyboxTexture->GetPath();
	}
	data["skyboxTexture"] = skyboxPath;

	return data;
}

void SkyboxRenderer::DeSerialize(const nlohmann::json& data)
{
	auto textureManager = TextureManager::Instance();
	UseSkybox = data["useSkybox"];
	SkyboxRotationSpeed.x = data["SkyboxRotationSpeed"]["x"];
	SkyboxRotationSpeed.y = data["SkyboxRotationSpeed"]["y"];
	SkyboxRotationSpeed.z = data["SkyboxRotationSpeed"]["z"];
	SkyboxRotationDirection.x = data["skyboxRotationDirection"]["x"];
	SkyboxRotationDirection.y = data["skyboxRotationDirection"]["y"];
	SkyboxRotationDirection.z = data["skyboxRotationDirection"]["z"];
	SkyboxTexture = data["skyboxTexture"] == "none" ? nullptr : textureManager->LoadImageTextureMap(data["skyboxTexture"]);
}