#include "DeferredRenderer.h"

<<<<<<< HEAD
void DeferredRenderer::Render(std::shared_ptr<Registry> registry)
{
	RenderDirectionLights(registry);
	RenderPointLightsStencil(registry);
	RenderPointLights(registry);
	RenderSpotLightsStencil(registry);
	RenderSpotLights(registry);
}

void DeferredRenderer::RenderDirectionLights(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
=======
void DeferredRenderer::Render(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager)
{
	RenderDirectionLights(registry, resouceManager);
	RenderPointLightsStencil(registry, resouceManager);
	RenderPointLights(registry, resouceManager);
	RenderSpotLightsStencil(registry, resouceManager);
	RenderSpotLights(registry, resouceManager);
}

void DeferredRenderer::RenderDirectionLights(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager)
{
	auto fbo = resouceManager->GetFbo("Main");
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	fbo->ActivateTexture(GL_COLOR_ATTACHMENT4);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
<<<<<<< HEAD
	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("DirLightData")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("DeferredDir");
=======
	resouceManager->GetUbo("CameraData")->BindBufferBase(0);
	resouceManager->GetSsbo("DirLightData")->BindBufferBase(1);
	auto program = resouceManager->GetProgram("DeferredDir");
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	program->Bind();
	program->SetTexture("colorTexture", 0, fbo->GetTextureID("color"));
	program->SetTexture("additionalTexture", 1, fbo->GetTextureID("additional"));
	program->SetTexture("normalTexture", 2, fbo->GetTextureID("normal"));
	program->SetTexture("depthTexture", 3, fbo->GetTextureID("depth"));
<<<<<<< HEAD
	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, registry->GetSize<DirlightComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();
=======
	resouceManager->GetGeometry("Cube")->Bind();
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, registry->GetSize<DirlightComponent>());
	resouceManager->GetGeometry("Cube")->UnBind();
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	program->UnBind();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

<<<<<<< HEAD
void DeferredRenderer::RenderPointLightsStencil(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
=======
void DeferredRenderer::RenderPointLightsStencil(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager)
{
	auto fbo = resouceManager->GetFbo("Main");
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	fbo->DeactivateTexture();
	fbo->ClearStencil();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, 0);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

<<<<<<< HEAD
	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
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
	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
=======
	resouceManager->GetUbo("CameraData")->BindBufferBase(0);
	resouceManager->GetSsbo("PointLightTransform")->BindBufferBase(1);
	auto program = resouceManager->GetProgram("DeferredStencil");
	program->Bind();
	resouceManager->GetGeometry("ProxySphere")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resouceManager->GetGeometry("ProxySphere")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, registry->GetSize<PointLightComponent>());
	resouceManager->GetGeometry("ProxySphere")->UnBind();
	program->UnBind();
}

void DeferredRenderer::RenderPointLights(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager)
{
	auto fbo = resouceManager->GetFbo("Main");
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	fbo->ActivateTexture(GL_COLOR_ATTACHMENT4);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

<<<<<<< HEAD
	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("PointLightTransform")->BindBufferBase(1);
	resourceManager->GetSsbo("PointLightData")->BindBufferBase(2);
	auto program = resourceManager->GetProgram("DeferredPoint");
=======
	resouceManager->GetUbo("CameraData")->BindBufferBase(0);
	resouceManager->GetSsbo("PointLightTransform")->BindBufferBase(1);
	resouceManager->GetSsbo("PointLightData")->BindBufferBase(2);
	auto program = resouceManager->GetProgram("DeferredPoint");
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	program->Bind();
	program->SetTexture("colorTexture", 0, fbo->GetTextureID("color"));
	program->SetTexture("additionalTexture", 1, fbo->GetTextureID("additional"));
	program->SetTexture("normalTexture", 2, fbo->GetTextureID("normal"));
	program->SetTexture("depthTexture", 3, fbo->GetTextureID("depth"));
	program->SetUniform("textureWidth", (float)fbo->GetSize().x);
	program->SetUniform("textureHeight", (float)fbo->GetSize().y);
<<<<<<< HEAD
	resourceManager->GetGeometry("ProxySphere")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resourceManager->GetGeometry("ProxySphere")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, registry->GetSize<PointLightComponent>());
	resourceManager->GetGeometry("ProxySphere")->UnBind();
=======
	resouceManager->GetGeometry("ProxySphere")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resouceManager->GetGeometry("ProxySphere")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, registry->GetSize<PointLightComponent>());
	resouceManager->GetGeometry("ProxySphere")->UnBind();
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	program->UnBind();

	glDisable(GL_STENCIL_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

<<<<<<< HEAD
void DeferredRenderer::RenderSpotLightsStencil(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
=======
void DeferredRenderer::RenderSpotLightsStencil(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager)
{
	auto fbo = resouceManager->GetFbo("Main");
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	fbo->DeactivateTexture();
	fbo->ClearStencil();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, 0);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

<<<<<<< HEAD
	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
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
	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
=======
	resouceManager->GetUbo("CameraData")->BindBufferBase(0);
	resouceManager->GetSsbo("SpotLightTransform")->BindBufferBase(1);

	auto program = resouceManager->GetProgram("DeferredStencil");
	program->Bind();
	resouceManager->GetGeometry("Cone")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resouceManager->GetGeometry("Cone")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, registry->GetSize<SpotLightComponent>());
	resouceManager->GetGeometry("Cone")->UnBind();
	program->UnBind();
}

void DeferredRenderer::RenderSpotLights(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager)
{
	auto fbo = resouceManager->GetFbo("Main");
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	fbo->ActivateTexture(GL_COLOR_ATTACHMENT4);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

<<<<<<< HEAD
	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("SpotLightTransform")->BindBufferBase(1);
	resourceManager->GetSsbo("SpotLightData")->BindBufferBase(2);
	auto program = resourceManager->GetProgram("DeferredSpot");
=======
	resouceManager->GetUbo("CameraData")->BindBufferBase(0);
	resouceManager->GetSsbo("SpotLightTransform")->BindBufferBase(1);
	resouceManager->GetSsbo("SpotLightData")->BindBufferBase(2);
	auto program = resouceManager->GetProgram("DeferredSpot");
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	program->Bind();
	program->SetTexture("colorTexture", 0, fbo->GetTextureID("color"));
	program->SetTexture("additionalTexture", 1, fbo->GetTextureID("additional"));
	program->SetTexture("normalTexture", 2, fbo->GetTextureID("normal"));
	program->SetTexture("depthTexture", 3, fbo->GetTextureID("depth"));
	program->SetUniform("textureWidth", (float)fbo->GetSize().x);
	program->SetUniform("textureHeight", (float)fbo->GetSize().y);
<<<<<<< HEAD
	resourceManager->GetGeometry("Cone")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resourceManager->GetGeometry("Cone")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, registry->GetSize<SpotLightComponent>());
	resourceManager->GetGeometry("Cone")->UnBind();
=======
	resouceManager->GetGeometry("Cone")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resouceManager->GetGeometry("Cone")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, registry->GetSize<SpotLightComponent>());
	resouceManager->GetGeometry("Cone")->UnBind();
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	program->UnBind();

	glDisable(GL_STENCIL_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}