#include "SkyboxRenderer.h"

void SkyboxRenderer::Render(std::shared_ptr<Registry> registry, std::shared_ptr<Camera> camera)
{
	auto textureManager = TextureManager::Instance();
	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4 });

	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);
	glDepthFunc(GL_LEQUAL);
	auto program = resourceManager->GetProgram("Skybox");
	program->Bind();
	program->SetUniform("model", glm::translate(camera->GetPosition()) * glm::scale(glm::vec3(-1.f)));
	program->SetTexture("skyboxTexture", 0, textureManager->LoadImageTexture("../Assets/Skybox.png")->GetTextureID());
	resourceManager->GetGeometry("Cube")->Bind();
	glDrawElements(GL_TRIANGLES, resourceManager->GetGeometry("Cube")->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();
	glDepthFunc(prevDepthFnc);
}
