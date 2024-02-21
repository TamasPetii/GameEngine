#include "BillboardRenderer.h"

void BillboardRenderer::Render(std::shared_ptr<Registry> registry)
{
	RenderDirLightBillboard(registry);
	RenderPointLightBillboard(registry);
	RenderSpotLightBillboard(registry);
	RenderAudioBillboard(registry);
}

void BillboardRenderer::RenderDirLightBillboard(std::shared_ptr<Registry> registry)
{
	static auto dirLightIcon = TextureManager::Instance()->LoadImageTexture("../Assets/DirLightIcon.png");

	auto resourceManager = ResourceManager::Instance();
	auto textureManager = TextureManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT3 });

	resourceManager->GetSsbo("DirLightBillboard")->BindBufferBase(0);
	auto program = resourceManager->GetProgram("Billboard");
	program->Bind();
	program->SetTexture("billboardTexture", 0, dirLightIcon->GetTextureID());
	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArrays(GL_POINTS, 0, registry->GetSize<DirlightComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();
}

void BillboardRenderer::RenderPointLightBillboard(std::shared_ptr<Registry> registry)
{
	static auto pointLightIcon = TextureManager::Instance()->LoadImageTexture("../Assets/PointLightIcon.png");

	auto resourceManager = ResourceManager::Instance();
	auto textureManager = TextureManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT3 });

	resourceManager->GetSsbo("PointLightBillboard")->BindBufferBase(0);
	auto program = resourceManager->GetProgram("Billboard");
	program->Bind();
	program->SetTexture("billboardTexture", 0, pointLightIcon->GetTextureID());
	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArrays(GL_POINTS, 0, registry->GetSize<PointLightComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();
}

void BillboardRenderer::RenderSpotLightBillboard(std::shared_ptr<Registry> registry)
{
	static auto spotLightIcon = TextureManager::Instance()->LoadImageTexture("../Assets/SpotLightIcon.png");

	auto resourceManager = ResourceManager::Instance();
	auto textureManager = TextureManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT3 });

	resourceManager->GetSsbo("SpotLightBillboard")->BindBufferBase(0);
	auto program = resourceManager->GetProgram("Billboard");
	program->Bind();
	program->SetTexture("billboardTexture", 0, spotLightIcon->GetTextureID());
	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArrays(GL_POINTS, 0, registry->GetSize<SpotLightComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();
}

void BillboardRenderer::RenderAudioBillboard(std::shared_ptr<Registry> registry)
{
}
