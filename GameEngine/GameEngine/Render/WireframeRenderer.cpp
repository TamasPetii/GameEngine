#include "WireframeRenderer.h"

<<<<<<< HEAD
void WireframeRenderer::Render(std::shared_ptr<Registry> registry)
{
	//RenderPointLightsVolume(registry);
	//RenderSpotLightsVolume(registry);
	//RenderDefaultCollider(registry);
	//RenderSphereCollider(registry);
	//RenderBvhAabb(registry);
}

void WireframeRenderer::RenderPointLightsVolume(std::shared_ptr<Registry> registry)
=======
void WireframeRenderer::Render(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager)
{
	//RenderPointLightsVolume(registry, resourceManager);
	//RenderSpotLightsVolume(registry, resourceManager);
	//RenderDefaultCollider(registry, resourceManager);
	//RenderSphereCollider(registry, resourceManager);
	RenderBvhAabb(registry, resourceManager);
}

void WireframeRenderer::RenderPointLightsVolume(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager)
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
{
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

<<<<<<< HEAD
	auto resourceManager = ResourceManager::Instance();
=======
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
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

<<<<<<< HEAD
void WireframeRenderer::RenderSpotLightsVolume(std::shared_ptr<Registry> registry)
=======
void WireframeRenderer::RenderSpotLightsVolume(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager)
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
{
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

<<<<<<< HEAD
	auto resourceManager = ResourceManager::Instance();
=======
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
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

<<<<<<< HEAD
void WireframeRenderer::RenderDefaultCollider(std::shared_ptr<Registry> registry)
=======
void WireframeRenderer::RenderDefaultCollider(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager)
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
{
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

<<<<<<< HEAD
	auto resourceManager = ResourceManager::Instance();
=======
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
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

<<<<<<< HEAD
void WireframeRenderer::RenderSphereCollider(std::shared_ptr<Registry> registry)
=======
void WireframeRenderer::RenderSphereCollider(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager)
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
{
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

<<<<<<< HEAD
	auto resourceManager = ResourceManager::Instance();
=======
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	auto fbo = resourceManager->GetFbo("Main");
	fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT4 });

	resourceManager->GetUbo("CameraData")->BindBufferBase(0);
	resourceManager->GetSsbo("SphereColliderTransform")->BindBufferBase(1);
	auto program = resourceManager->GetProgram("Wireframe");
	program->Bind();
	program->SetUniform("u_color", glm::vec3(0, 1, 0));
	resourceManager->GetGeometry("ProxySphere")->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, resourceManager->GetGeometry("ProxySphere")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, registry->GetSize<SphereCollider>());
	resourceManager->GetGeometry("ProxySphere")->UnBind();
	program->UnBind();

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

<<<<<<< HEAD
void WireframeRenderer::RenderBvhAabb(std::shared_ptr<Registry> registry)
=======
void WireframeRenderer::RenderBvhAabb(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager)
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
{
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

<<<<<<< HEAD
	auto resourceManager = ResourceManager::Instance();
=======
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
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
}