#include "SkyboxRenderer.h"

void SkyboxRenderer::Render(std::shared_ptr<Registry> registry)
{
	if (!GlobalSettings::SkyboxTexture)
		GlobalSettings::SkyboxTexture = TextureManager::Instance()->LoadImageTextureMap("../Assets/sky.png");
		
	if (!GlobalSettings::UseSkybox)
		return;

	GlobalSettings::SkyboxRotationMatrix = glm::rotate(glm::radians(GlobalSettings::SkyboxRotation.z), glm::vec3(0, 0, 1))
											* glm::rotate(glm::radians(GlobalSettings::SkyboxRotation.y), glm::vec3(0, 1, 0))
											* glm::rotate(glm::radians(GlobalSettings::SkyboxRotation.x), glm::vec3(1, 0, 0));

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
	program->SetUniform("model", glm::translate(glm::vec3(cameraComponent.position)) * GlobalSettings::SkyboxRotationMatrix * glm::scale(glm::vec3(-1.f)));
	program->SetTexture("skyboxTexture", 0, GlobalSettings::SkyboxTexture->GetTextureID());

	resourceManager->GetGeometry("Cube")->Bind();
	glDrawElements(GL_TRIANGLES, resourceManager->GetGeometry("Cube")->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();
	glDepthFunc(prevDepthFnc);

	fbo->UnBind();
}
