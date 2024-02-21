#include "Renderer.h"

void Renderer::RenderScene(std::shared_ptr<Scene> scene)
{
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	auto resourceManager = ResourceManager::Instance();

	auto fbo = resourceManager->GetFbo("Main");
	fbo->Clear();
	fbo->Bind();

	ShadowRenderer::Render(scene->GetRegistry());
	GeometryRenderer::Render(scene->GetRegistry());
	DeferredRenderer::Render(scene->GetRegistry());
	BillboardRenderer::Render(scene->GetRegistry());
	WireframeRenderer::Render(scene->GetRegistry());
	SkyboxRenderer::Render(scene->GetRegistry(), scene->GetMainCamera());
	BloomRenderer::Render(scene->GetRegistry());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}