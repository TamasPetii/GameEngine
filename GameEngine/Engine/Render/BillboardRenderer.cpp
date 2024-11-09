#include "BillboardRenderer.h"

void BillboardRenderer::Render(std::shared_ptr<Registry> registry)
{
	if (GlobalSettings::GameViewActive)
		return;

	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->Bind();
	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);

	RenderDirLightBillboard(registry);
	RenderPointLightBillboard(registry);
	RenderSpotLightBillboard(registry);
	RenderAudioBillboard(registry);
	RenderCameraBillboard(registry);

	fbo->UnBind();
}

void BillboardRenderer::RenderDirLightBillboard(std::shared_ptr<Registry> registry)
{
	if (!registry->GetComponentPool<DirlightComponent>())
		return;

	static auto dirLightIcon = TextureManager::Instance()->LoadImageTexture("../Assets/DirLightIcon.png");

	auto resourceManager = ResourceManager::Instance();
	auto textureManager = TextureManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT3 });

	resourceManager->GetSsbo("DirLightBillboard")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("Billboard");
	program->Bind();
	program->SetUniform("u_cullIndex", null);
	program->SetTexture("billboardTexture", 0, dirLightIcon->GetTextureID());
	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArrays(GL_POINTS, 0, registry->GetSize<DirlightComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();
}

void BillboardRenderer::RenderPointLightBillboard(std::shared_ptr<Registry> registry)
{
	if (!registry->GetComponentPool<PointLightComponent>())
		return;

	static auto pointLightIcon = TextureManager::Instance()->LoadImageTexture("../Assets/PointLightIcon.png");

	auto resourceManager = ResourceManager::Instance();
	auto textureManager = TextureManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT3 });

	resourceManager->GetSsbo("PointLightBillboard")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("Billboard");
	program->Bind();
	program->SetUniform("u_cullIndex", null);
	program->SetTexture("billboardTexture", 0, pointLightIcon->GetTextureID());
	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArrays(GL_POINTS, 0, registry->GetSize<PointLightComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();
}

void BillboardRenderer::RenderSpotLightBillboard(std::shared_ptr<Registry> registry)
{
	if (!registry->GetComponentPool<SpotLightComponent>())
		return;

	static auto spotLightIcon = TextureManager::Instance()->LoadImageTexture("../Assets/SpotLightIcon.png");

	auto resourceManager = ResourceManager::Instance();
	auto textureManager = TextureManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT3 });

	resourceManager->GetSsbo("SpotLightBillboard")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("Billboard");
	program->Bind();
	program->SetUniform("u_cullIndex", null);
	program->SetTexture("billboardTexture", 0, spotLightIcon->GetTextureID());

	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArrays(GL_POINTS, 0, registry->GetSize<SpotLightComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();
}

void BillboardRenderer::RenderAudioBillboard(std::shared_ptr<Registry> registry)
{
	if (!registry->GetComponentPool<AudioComponent>())
		return;

	static auto soundLightIcon = TextureManager::Instance()->LoadImageTexture("../Assets/SoundIcon.png");

	auto resourceManager = ResourceManager::Instance();
	auto textureManager = TextureManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT3 });

	resourceManager->GetSsbo("AudioBillboard")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("Billboard");
	program->Bind();
	program->SetUniform("u_cullIndex", null);
	program->SetTexture("billboardTexture", 0, soundLightIcon->GetTextureID());

	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArrays(GL_POINTS, 0, registry->GetSize<AudioComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();
}

void BillboardRenderer::RenderCameraBillboard(std::shared_ptr<Registry> registry)
{
	if (!registry->GetComponentPool<CameraComponent>())
		return;

	static auto cameraLightIcon = TextureManager::Instance()->LoadImageTexture("../Assets/CameraIcon.png");

	auto resourceManager = ResourceManager::Instance();
	auto textureManager = TextureManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT3 });

	resourceManager->GetSsbo("CameraBillboardData")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("Billboard");
	program->Bind();
	program->SetUniform("u_cullIndex", CameraSystem::GetMainCameraIndex(registry));
	program->SetTexture("billboardTexture", 0, cameraLightIcon->GetTextureID());

	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArrays(GL_POINTS, 0, registry->GetSize<CameraComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();
}
