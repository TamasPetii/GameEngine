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
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeID<TransformSystem>()]);

		ImGui::Text("Material System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeID<MaterialSystem>()]);

		ImGui::Text("Dir Light System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeID<DirlightSystem>()]);

		ImGui::Text("Point Light System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeID<PointLightSystem>()]);

		ImGui::Text("Spot Light System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeID<SpotLightSystem>()]);

		ImGui::Text("Default Collider System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeID<DefaultColliderSystem>()]);

		ImGui::Text("Sphere Collider System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeID<SphereColliderSystem>()]);

		ImGui::Text("Mesh Collider System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeID<MeshColliderSystem>()]);

		ImGui::Text("Bvh System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeID<BvhSystem>()]);

		ImGui::Text("Collision Test System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeID<CollisionSystem>()]);

		ImGui::Text("Collision Resolve System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeID<CollisionResponseSystem>()]);

		ImGui::Text("Frustum Culling System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeID<FrustumCullingSystem>()]);

		ImGui::Text("Instance System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeID<InstanceSystem>()]);

		ImGui::Text("Physics System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeID<PhysicsSystem>()]);
	}
}