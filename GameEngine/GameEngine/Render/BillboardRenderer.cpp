#include "BillboardRenderer.h"

<<<<<<< HEAD
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
=======
void BillboardRenderer::Render(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<AssetManager> assetManager)
{
	RenderDirLightBillboard(registry, resourceManager, assetManager);
	RenderPointLightBillboard(registry, resourceManager, assetManager);
	RenderSpotLightBillboard(registry, resourceManager, assetManager);
	RenderAudioBillboard(registry, resourceManager, assetManager);
}

void BillboardRenderer::RenderDirLightBillboard(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<AssetManager> assetManager)
{
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT3 });

	resourceManager->GetSsbo("DirLightBillboard")->BindBufferBase(0);
	auto program = resourceManager->GetProgram("Billboard");
	program->Bind();
<<<<<<< HEAD
	program->SetTexture("billboardTexture", 0, dirLightIcon->GetTextureID());
=======
	program->SetTexture("billboardTexture", 0, assetManager->GetImageTexture("../Assets/DirLightIcon.png")->GetTextureID());
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArrays(GL_POINTS, 0, registry->GetSize<DirlightComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();
}

<<<<<<< HEAD
void BillboardRenderer::RenderPointLightBillboard(std::shared_ptr<Registry> registry)
{
	static auto pointLightIcon = TextureManager::Instance()->LoadImageTexture("../Assets/PointLightIcon.png");

	auto resourceManager = ResourceManager::Instance();
	auto textureManager = TextureManager::Instance();
=======
void BillboardRenderer::RenderPointLightBillboard(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<AssetManager> assetManager)
{
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT3 });

	resourceManager->GetSsbo("PointLightBillboard")->BindBufferBase(0);
	auto program = resourceManager->GetProgram("Billboard");
	program->Bind();
<<<<<<< HEAD
	program->SetTexture("billboardTexture", 0, pointLightIcon->GetTextureID());
=======
	program->SetTexture("billboardTexture", 0, assetManager->GetImageTexture("../Assets/PointLightIcon.png")->GetTextureID());
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArrays(GL_POINTS, 0, registry->GetSize<PointLightComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();
}

<<<<<<< HEAD
void BillboardRenderer::RenderSpotLightBillboard(std::shared_ptr<Registry> registry)
{
	static auto spotLightIcon = TextureManager::Instance()->LoadImageTexture("../Assets/SpotLightIcon.png");

	auto resourceManager = ResourceManager::Instance();
	auto textureManager = TextureManager::Instance();
=======
void BillboardRenderer::RenderSpotLightBillboard(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<AssetManager> assetManager)
{
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT3 });

	resourceManager->GetSsbo("SpotLightBillboard")->BindBufferBase(0);
	auto program = resourceManager->GetProgram("Billboard");
	program->Bind();
<<<<<<< HEAD
	program->SetTexture("billboardTexture", 0, spotLightIcon->GetTextureID());
=======
	program->SetTexture("billboardTexture", 0, assetManager->GetImageTexture("../Assets/SpotLightIcon.png")->GetTextureID());
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArrays(GL_POINTS, 0, registry->GetSize<SpotLightComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();
}

<<<<<<< HEAD
void BillboardRenderer::RenderAudioBillboard(std::shared_ptr<Registry> registry)
=======
void BillboardRenderer::RenderAudioBillboard(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<AssetManager> assetManager)
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
{
}
