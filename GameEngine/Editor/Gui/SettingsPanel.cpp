#include "SettingsPanel.h"

void SettingsPanel::Update(std::shared_ptr<Scene> scene)
{

}

void SettingsPanel::Render(std::shared_ptr<Scene> scene)
{
	if (ImGui::Begin(TITLE_SP("SettingsPanel")))
	{
		RenderTextures();
		RenderBloomTextures();
		RenderCollisionStats();
		RenderSystemTimes(scene);
		RenderRenderTimes();
	}

	ImGui::End();
}

void SettingsPanel::RenderTextures()
{
    if (ImGui::CollapsingHeader(TITLE_SP("Framebuffer Textures")))
    {
		auto resourceManager = ResourceManager::Instance();
		auto fbo = resourceManager->GetFbo("Main");
		ImGui::Image((ImTextureID)fbo->GetTextureID("color"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("normal"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("bloom"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("main"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
    }
}

void SettingsPanel::RenderBloomTextures()
{
	if (ImGui::CollapsingHeader(TITLE_SP("Bloom Textures")))
	{
		auto resourceManager = ResourceManager::Instance();
		auto fbo = resourceManager->GetFbo("Bloom");
		ImGui::Checkbox(TITLE_SP("Use Bloom"), &BloomRenderer::useBloom);
		ImGui::DragFloat(TITLE_SP("Gamma"),    &BloomRenderer::gamma, 0.005f);
		ImGui::DragFloat(TITLE_SP("Exposure"), &BloomRenderer::exposure, 0.005f);
		ImGui::Image((ImTextureID)fbo->GetTextureID("bloom0"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("bloom1"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("bloom2"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("bloom3"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("bloom4"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("bloom5"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
	}
}

void SettingsPanel::RenderCollisionStats()
{
	if (ImGui::CollapsingHeader(TITLE_SP("Collision Stats")))
	{
		ImGui::Text("NAIVE Collision Test = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "%d", CollisionSystem::naiveCounter);

		ImGui::Text("AABB Collision Test with BVH = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 0, 1, 1), "%d", CollisionSystem::aabbCounter.load());

		ImGui::Text("GJK Collision Test with BVH = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "%d", CollisionSystem::gjkCounter.load());

		ImGui::Text("GJK Collision Test Success = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 1, 1), "%d", CollisionSystem::gjkSuccess.load());
	}
}

void SettingsPanel::RenderSystemTimes(std::shared_ptr<Scene> scene)
{
	if (ImGui::CollapsingHeader(TITLE_SP("System Time Stats")))
	{
		auto& systemTimes = scene->GetSystemTimes();

		ImGui::Text("Transform System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<TransformSystem>()]);

		ImGui::Text("Material System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<MaterialSystem>()]);

		ImGui::Text("Dir Light System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<DirlightSystem>()]);

		ImGui::Text("Point Light System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<PointLightSystem>()]);

		ImGui::Text("Spot Light System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<SpotLightSystem>()]);

		ImGui::Text("Default Collider System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<DefaultColliderSystem>()]);

		ImGui::Text("Sphere Collider System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<SphereColliderSystem>()]);

		ImGui::Text("Mesh Collider System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<MeshColliderSystem>()]);

		ImGui::Text("Bvh System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<BvhSystem>()]);

		ImGui::Text("Collision Test System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<CollisionSystem>()]);

		ImGui::Text("Collision Resolve System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<CollisionResponseSystem>()]);

		ImGui::Text("Frustum Culling System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<FrustumCullingSystem>()]);

		ImGui::Text("Instance System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<InstanceSystem>()]);

		ImGui::Text("Physics System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<PhysicsSystem>()]);

		ImGui::Text("Animation System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<AnimationSystem>()]);
	}
}

void SettingsPanel::RenderRenderTimes()
{
	if (ImGui::CollapsingHeader(TITLE_SP("Render Time Stats")))
	{
		auto& renderTimes = Renderer::m_AverageRenderTimes;

		ImGui::Text("Shadow Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<ShadowRenderer>()]);

		ImGui::Text("GeometryRenderer Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<GeometryRenderer>()]);

		ImGui::Text("DeferredRenderer Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<DeferredRenderer>()]);

		ImGui::Text("BillboardRenderer Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<BillboardRenderer>()]);

		ImGui::Text("WireframeRenderer Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<WireframeRenderer>()]);

		ImGui::Text("SkyboxRenderer Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<SkyboxRenderer>()]);

		ImGui::Text("BloomRenderer Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<BloomRenderer>()]);
	}
}