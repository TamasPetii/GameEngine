#include "WireframeRenderer.h"

#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"

#include "Render/Geometry/Geometry.h"
#include "Render/OpenGL/ProgramGL.h"
#include "Render/OpenGL/FramebufferGL.h"
#include "Render/OpenGL/ShaderStorageBufferGL.h"

#include "Registry/Component/Light/DirlightComponent.h"
#include "Registry/Component/Light/PointlightComponent.h"
#include "Registry/Component/Light/SpotLightComponent.h"
#include "Registry/Component/Physics/DefaultCollider.h"
#include "Registry/Component/Physics/SphereColliderComponent.h"
#include "Registry/Component/Physics/BoxColliderComponent.h"
#include "Registry/Component/CameraComponent.h"
#include "Registry/System/CameraSystem.h"
#include "Settings/GlobalSettings.h"

bool WireframeRenderer::ShowDirLightsLines = false;
bool WireframeRenderer::ShowPointLightsVolume = false;
bool WireframeRenderer::ShowSpotLightsVolume = false;
bool WireframeRenderer::ShowDefaultCollider = false;
bool WireframeRenderer::ShowBoxCollider = false;
bool WireframeRenderer::ShowSphereCollider = false;
bool WireframeRenderer::ShowCameraVolume = false;

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

	if(ShowCameraVolume)
		RenderCameraVolume(registry);

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

	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("DirLightLines")->BindBufferBase(1);

	auto program = resourceManager->GetProgram("DirLightLine");
	program->Bind();
	program->SetUniform("u_cullIndex", null);
	program->SetUniform("u_renderMode", (unsigned int)0);
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

	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("PointLightTransform")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("Wireframe");
	program->Bind();
	program->SetUniform("u_cullIndex", null);
	program->SetUniform("u_renderMode", (unsigned int)0);
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

	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("SpotLightTransform")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("Wireframe");
	program->Bind();
	program->SetUniform("u_cullIndex", null);
	program->SetUniform("u_renderMode", (unsigned int)0);
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

	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("DefaultColliderTransform")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("Wireframe");
	program->Bind();
	program->SetUniform("u_cullIndex", null);
	program->SetUniform("u_renderMode", (unsigned int)0);
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

	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("BoxColliderTransform")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("Wireframe");
	program->Bind();
	program->SetUniform("u_cullIndex", null);
	program->SetUniform("u_renderMode", (unsigned int)0);
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

	resourceManager->GetSsbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("SphereColliderTransform")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("Wireframe");
	program->Bind();
	program->SetUniform("u_cullIndex", null);
	program->SetUniform("u_renderMode", (unsigned int)0);
	program->SetUniform("u_color", glm::vec3(0, 1, 0));
	resourceManager->GetGeometry("Sphere")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resourceManager->GetGeometry("Sphere")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, registry->GetSize<SphereColliderComponent>());
	resourceManager->GetGeometry("Sphere")->UnBind();
	program->UnBind();

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void WireframeRenderer::RenderCameraVolume(std::shared_ptr<Registry> registry)
{
	if (!registry->GetComponentPool<CameraComponent>())
		return;

	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4 });
	resourceManager->GetSsbo("CameraWireframeData")->BindBufferBase(2);

	auto program = resourceManager->GetProgram("Wireframe");
	program->Bind();
	program->SetUniform("u_cullIndex", CameraSystem::GetMainCameraIndex(registry));
	program->SetUniform("u_renderMode", (unsigned int)1);
	program->SetUniform("u_color", glm::vec3(0, 1, 1));
	resourceManager->GetGeometry("Cube")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resourceManager->GetGeometry("Cube")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, registry->GetSize<CameraComponent>());
	resourceManager->GetGeometry("Cube")->UnBind();
	program->UnBind();

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}