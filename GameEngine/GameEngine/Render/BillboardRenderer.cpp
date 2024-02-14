#include "BillboardRenderer.h"

void BillboardRenderer::Render(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<AssetManager> assetManager)
{
	RenderDirLightBillboard(registry, resourceManager, assetManager);
	RenderPointLightBillboard(registry, resourceManager, assetManager);
	RenderSpotLightBillboard(registry, resourceManager, assetManager);
	RenderAudioBillboard(registry, resourceManager, assetManager);
}

void BillboardRenderer::RenderDirLightBillboard(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<AssetManager> assetManager)
{
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT3 });

	resourceManager->GetSsbo("DirLightBillboard")->BindBufferBase(0);
	auto program = resourceManager->GetProgram("Billboard");
	program->Bind();
	program->SetTexture("billboardTexture", 0, assetManager->GetImageTexture("../Assets/DirLightIcon.png")->GetTextureID());
	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArrays(GL_POINTS, 0, registry->GetSize<DirlightComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();
}

void BillboardRenderer::RenderPointLightBillboard(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<AssetManager> assetManager)
{
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT3 });

	resourceManager->GetSsbo("PointLightBillboard")->BindBufferBase(0);
	auto program = resourceManager->GetProgram("Billboard");
	program->Bind();
	program->SetTexture("billboardTexture", 0, assetManager->GetImageTexture("../Assets/PointLightIcon.png")->GetTextureID());
	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArrays(GL_POINTS, 0, registry->GetSize<PointLightComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();
}

void BillboardRenderer::RenderSpotLightBillboard(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<AssetManager> assetManager)
{
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT3 });

	resourceManager->GetSsbo("SpotLightBillboard")->BindBufferBase(0);
	auto program = resourceManager->GetProgram("Billboard");
	program->Bind();
	program->SetTexture("billboardTexture", 0, assetManager->GetImageTexture("../Assets/SpotLightIcon.png")->GetTextureID());
	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArrays(GL_POINTS, 0, registry->GetSize<SpotLightComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();
}

void BillboardRenderer::RenderAudioBillboard(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<AssetManager> assetManager)
{
}
