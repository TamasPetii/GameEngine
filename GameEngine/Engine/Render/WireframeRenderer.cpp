#include "WireframeRenderer.h"

bool WireframeRenderer::ShowDirLightsLines = false;
bool WireframeRenderer::ShowPointLightsVolume = false;
bool WireframeRenderer::ShowSpotLightsVolume = false;
bool WireframeRenderer::ShowDefaultCollider = false;
bool WireframeRenderer::ShowBoxCollider = false;
bool WireframeRenderer::ShowSphereCollider = false;
bool WireframeRenderer::ShowBvhBoxes = false;

void WireframeRenderer::Render(std::shared_ptr<Registry> registry)
{
	if (GlobalSettings::GameViewActive)
		return;

	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->Bind();

	if (ShowDirLightsLines)
		RenderDirLightsLine(registry);

	if(ShowPointLightsVolume)
		RenderPointLightsVolume(registry);
	
	if(ShowSpotLightsVolume)
		RenderSpotLightsVolume(registry);

	if(ShowDefaultCollider)
		RenderDefaultCollider(registry);

	if (ShowBoxCollider)
		RenderBoxCollider(registry);

	if(ShowSphereCollider)
		RenderSphereCollider(registry);

	if(ShowBvhBoxes)
		RenderBvhAabb(registry);

	fbo->UnBind();
}

void WireframeRenderer::RenderDirLightsLine(std::shared_ptr<Registry> registry)
{
	if (!registry->GetComponentPool<DirlightComponent>())
		return;

	glLineWidth(3);

	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4 });

	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("DirLightLines")->BindBufferBase(1);

	auto program = resourceManager->GetProgram("DirLightLine");
	program->Bind();
	program->SetUniform("u_color", glm::vec3(1, 1, 0));

	resourceManager->GetGeometry("Cube")->Bind();
	glDrawArraysInstanced(GL_LINES, 0, 2, registry->GetSize<DirlightComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();

	glLineWidth(1);

	program->UnBind();
}

void WireframeRenderer::RenderPointLightsVolume(std::shared_ptr<Registry> registry)
{
	if (!registry->GetComponentPool<PointLightComponent>())
		return;

	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4 });

	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("PointLightTransform")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("Wireframe");
	program->Bind();
	program->SetUniform("u_color", glm::vec3(0,0,1));
	resourceManager->GetGeometry("ProxySphere")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resourceManager->GetGeometry("ProxySphere")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, registry->GetSize<PointLightComponent>());
	resourceManager->GetGeometry("ProxySphere")->UnBind();
	program->UnBind();

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void WireframeRenderer::RenderSpotLightsVolume(std::shared_ptr<Registry> registry)
{
	if (!registry->GetComponentPool<SpotLightComponent>())
		return;

	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4 });

	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("SpotLightTransform")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("Wireframe");
	program->Bind();
	program->SetUniform("u_color", glm::vec3(1, 1, 0));
	resourceManager->GetGeometry("Cone")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resourceManager->GetGeometry("Cone")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, registry->GetSize<SpotLightComponent>());
	resourceManager->GetGeometry("Cone")->UnBind();
	program->UnBind();

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void WireframeRenderer::RenderDefaultCollider(std::shared_ptr<Registry> registry)
{
	if (!registry->GetComponentPool<DefaultCollider>())
		return;

	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4 });

	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("DefaultColliderTransform")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("Wireframe");
	program->Bind();
	program->SetUniform("u_color", glm::vec3(1, 0, 0));
	resourceManager->GetGeometry("Cube")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resourceManager->GetGeometry("Cube")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, registry->GetSize<DefaultCollider>());
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void WireframeRenderer::RenderBoxCollider(std::shared_ptr<Registry> registry)
{
	if (!registry->GetComponentPool<BoxColliderComponent>())
		return;

	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4 });

	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("BoxColliderTransform")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("Wireframe");
	program->Bind();
	program->SetUniform("u_color", glm::vec3(0, 1, 0));
	resourceManager->GetGeometry("Cube")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resourceManager->GetGeometry("Cube")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, registry->GetSize<BoxColliderComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void WireframeRenderer::RenderSphereCollider(std::shared_ptr<Registry> registry)
{
	if (!registry->GetComponentPool<SphereColliderComponent>())
		return;

	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4 });

	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("SphereColliderTransform")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("Wireframe");
	program->Bind();
	program->SetUniform("u_color", glm::vec3(0, 1, 0));
	resourceManager->GetGeometry("Sphere")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resourceManager->GetGeometry("Sphere")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, registry->GetSize<SphereColliderComponent>());
	resourceManager->GetGeometry("Sphere")->UnBind();
	program->UnBind();

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void WireframeRenderer::RenderBvhAabb(std::shared_ptr<Registry> registry)
{
	/*
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4 });

	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("BvhTransformData")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("Wireframe");
	program->Bind();
	program->SetUniform("u_color", glm::vec3(0, 1, 1));
	resourceManager->GetGeometry("Cube")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resourceManager->GetGeometry("Cube")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, BvhSystem::index);
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	*/
}