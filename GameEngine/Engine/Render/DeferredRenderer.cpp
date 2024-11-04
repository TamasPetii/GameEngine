#include "DeferredRenderer.h"

void DeferredRenderer::Render(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->Bind();

	RenderAmbientColors(registry);
	RenderDirectionLights(registry);
	RenderPointLights(registry);
	RenderSpotLights(registry);

	fbo->UnBind();
}

void DeferredRenderer::RenderAmbientColors(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTexture(GL_COLOR_ATTACHMENT4);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	auto program = resourceManager->GetProgram("DeferredAmbient");
	program->Bind();
	program->SetTexture("colorTexture", 0, fbo->GetTextureID("color"));

	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void DeferredRenderer::RenderDirectionLights(std::shared_ptr<Registry> registry)
{
	if (!registry->GetComponentPool<DirlightComponent>())
		return;

	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTexture(GL_COLOR_ATTACHMENT4);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("DirLightData")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("DeferredDir");
	program->Bind();
	program->SetTexture("colorTexture", 0, fbo->GetTextureID("color"));
	program->SetTexture("additionalTexture", 1, fbo->GetTextureID("additional"));
	program->SetTexture("normalTexture", 2, fbo->GetTextureID("normal"));
	program->SetTexture("depthTexture", 3, fbo->GetTextureID("depth"));
	program->SetTexture("positionTexture", 4, fbo->GetTextureID("position"));

	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, registry->GetSize<DirlightComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

//This does not work properly for instanced rendered spheres
void DeferredRenderer::RenderPointLightsStencil(std::shared_ptr<Registry> registry)
{
	if (!registry->GetComponentPool<DirlightComponent>())
		return;

	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");

	fbo->DeactivateTexture();
	fbo->ClearStencil();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, 0);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("PointLightTransform")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("DeferredStencil");
	program->Bind();
	resourceManager->GetGeometry("ProxySphere")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resourceManager->GetGeometry("ProxySphere")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, registry->GetSize<PointLightComponent>());
	resourceManager->GetGeometry("ProxySphere")->UnBind();
	program->UnBind();
}

void DeferredRenderer::RenderPointLights(std::shared_ptr<Registry> registry)
{
	auto pointLightPool = registry->GetComponentPool<PointLightComponent>();
	if (!pointLightPool)
		return;
	
	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");

	fbo->ActivateTexture(GL_COLOR_ATTACHMENT4);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("PointLightTransform")->BindBufferBase(1);
	resourceManager->GetSsbo("PointLightData")->BindBufferBase(2);
	auto program = resourceManager->GetProgram("DeferredPoint");

	program->Bind();
	program->SetTexture("colorTexture", 0, fbo->GetTextureID("color"));
	program->SetTexture("additionalTexture", 1, fbo->GetTextureID("additional"));
	program->SetTexture("normalTexture", 2, fbo->GetTextureID("normal"));
	program->SetTexture("depthTexture", 3, fbo->GetTextureID("depth"));
	program->SetTexture("positionTexture", 4, fbo->GetTextureID("position"));

	program->SetUniform("textureWidth", (float)fbo->GetSize().x);
	program->SetUniform("textureHeight", (float)fbo->GetSize().y);
	program->SetUniform("bias", GlobalSettings::bias);

	resourceManager->GetGeometry("Sphere")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resourceManager->GetGeometry("Sphere")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, registry->GetSize<PointLightComponent>());
	resourceManager->GetGeometry("Sphere")->UnBind();

	program->UnBind();

	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

//This does not work properly for instanced rendered spheres
//glEnable(GL_STENCIL_TEST);
//glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
void DeferredRenderer::RenderSpotLightsStencil(std::shared_ptr<Registry> registry)
{
	if (!registry->GetComponentPool<PointLightComponent>())
		return;

	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");

	fbo->DeactivateTexture();
	fbo->ClearStencil();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, 0);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("SpotLightTransform")->BindBufferBase(1);

	auto program = resourceManager->GetProgram("DeferredStencil");
	program->Bind();
	resourceManager->GetGeometry("Cone")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resourceManager->GetGeometry("Cone")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, registry->GetSize<SpotLightComponent>());
	resourceManager->GetGeometry("Cone")->UnBind();
	program->UnBind();
}

void DeferredRenderer::RenderSpotLights(std::shared_ptr<Registry> registry)
{
	if (!registry->GetComponentPool<SpotLightComponent>())
		return;

	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");

	fbo->ActivateTexture(GL_COLOR_ATTACHMENT4);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("SpotLightTransform")->BindBufferBase(1);
	resourceManager->GetSsbo("SpotLightData")->BindBufferBase(2);
	auto program = resourceManager->GetProgram("DeferredSpot");

	program->Bind();
	program->SetTexture("colorTexture", 0, fbo->GetTextureID("color"));
	program->SetTexture("additionalTexture", 1, fbo->GetTextureID("additional"));
	program->SetTexture("normalTexture", 2, fbo->GetTextureID("normal"));
	program->SetTexture("depthTexture", 3, fbo->GetTextureID("depth"));
	program->SetTexture("positionTexture", 4, fbo->GetTextureID("position"));

	program->SetUniform("textureWidth", (float)fbo->GetSize().x);
	program->SetUniform("textureHeight", (float)fbo->GetSize().y);
	//program->SetUniform("bias", GlobalSettings::bias);

	resourceManager->GetGeometry("Cone")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resourceManager->GetGeometry("Cone")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, registry->GetSize<SpotLightComponent>());
	resourceManager->GetGeometry("Cone")->UnBind();
	program->UnBind();

	glDisable(GL_STENCIL_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}