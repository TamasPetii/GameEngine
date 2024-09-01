#include "SettingsPanel.h"

void SettingsPanel::Update(std::shared_ptr<Scene> scene)
{

}

void SettingsPanel::Render(std::shared_ptr<Scene> scene)
{
	if (GlobalSettings::GameViewActive)
		return;

	if (ImGui::Begin(TITLE_SP("SettingsPanel")))
	{
		RenderTextures(scene);
		RenderBloomTextures();
		RenderSystemTimes(scene);
		RenderRenderTimes();
		RenderWireframeSettings();
	}

	ImGui::End();
}

void SettingsPanel::RenderTextures(std::shared_ptr<Scene> scene)
{
    if (ImGui::CollapsingHeader(TITLE_SP("Framebuffer Textures")))
    {
		auto resourceManager = ResourceManager::Instance();
		auto fbo = resourceManager->GetFbo("Main");
		ImGui::Image((ImTextureID)fbo->GetTextureID("color"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("normal"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("bloom"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("main"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));

		for (auto& [name, texture] : PreviewManager::Instance()->RefAnimationPreviews())
		{
			if (texture)
				ImGui::Image((ImTextureID)texture->GetTextureID(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
		}
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

void SettingsPanel::RenderSystemTimes(std::shared_ptr<Scene> scene)
{
	if (ImGui::CollapsingHeader(TITLE_SP("System Time Stats")))
	{
		auto& systemTimes = scene->GetSystemTimes();

		ImGui::SeparatorText("Physics simulation");

		ImGui::Text("Physics System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<PhysicsSystem>()]);

		ImGui::Text("Dynamic Rigidbody System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<RigidbodyDynamicSystem>()]);

		ImGui::Text("Static Rigidbody System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<RigidbodyStaticSystem>()]);

		ImGui::Text("Default Collider System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<DefaultColliderSystem>()]);

		ImGui::Text("Box Collider System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<BoxColliderSystem>()]);

		ImGui::Text("Sphere Collider System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<SphereColliderSystem>()]);

		ImGui::Text("Convex Collider System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<ConvexColliderSystem>()]);
		
		ImGui::Text("Mesh Collider System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<MeshColliderSystem>()]);

		ImGui::SeparatorText("Light update");

		ImGui::Text("Dir Light System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<DirlightSystem>()]);

		ImGui::Text("Point Light System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<PointLightSystem>()]);

		ImGui::Text("Spot Light System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<SpotLightSystem>()]);

		ImGui::SeparatorText("Geometry update");

		ImGui::Text("Shape System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<ShapeSystem>()]);

		ImGui::Text("Model System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<ModelSystem>()]);

		ImGui::Text("Animation System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<AnimationSystem>()]);

		ImGui::Text("Material System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<MaterialSystem>()]);

		ImGui::SeparatorText("Render preparation");

		ImGui::Text("Transform System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<TransformSystem>()]);

		ImGui::Text("Frustum Culling System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<FrustumCullingSystem>()]);

		ImGui::Text("Instance System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<InstanceSystem>()]);

		ImGui::Text("Water System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<WaterSystem>()]);

		ImGui::SeparatorText("Others");

		ImGui::Text("Audio System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<AudioSystem>()]);
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

		ImGui::Text("Geometry Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<GeometryRenderer>()]);

		ImGui::Text("Deferred Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<DeferredRenderer>()]);

		ImGui::Text("Billboard Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<BillboardRenderer>()]);

		ImGui::Text("Wireframe Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<WireframeRenderer>()]);

		ImGui::Text("Skybox Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<SkyboxRenderer>()]);

		ImGui::Text("Bloom Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<BloomRenderer>()]);

		ImGui::Text("Water Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<WaterRenderer>()]);

		ImGui::Text("Preview Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<PreviewRenderer>()]);
	}
}

void SettingsPanel::RenderWireframeSettings()
{
	if (ImGui::CollapsingHeader(TITLE_SP("Wireframe Settings")))
	{

		ImGui::Text("Show Dirlight Lines");
		ImGui::SameLine();
		ImGui::Checkbox("##Show Dirlight Lines", &WireframeRenderer::ShowDirLightsLines);

		ImGui::Text("Show Pointlight Volume");
		ImGui::SameLine();
		ImGui::Checkbox("##Show Pointlight Volume", &WireframeRenderer::ShowPointLightsVolume);
		
		ImGui::Text("Show Spotlight Volume");
		ImGui::SameLine();
		ImGui::Checkbox("##Show Spotlight Volume", &WireframeRenderer::ShowSpotLightsVolume);

		ImGui::Text("Show Default Collider Volume");
		ImGui::SameLine();
		ImGui::Checkbox("##Show Default Collider Volume", &WireframeRenderer::ShowDefaultCollider);

		ImGui::Text("Show Sphere Collider Volume");
		ImGui::SameLine();
		ImGui::Checkbox("##Show Sphere Collider Volume", &WireframeRenderer::ShowSphereCollider);

		ImGui::Text("Show Bvh Volume");
		ImGui::SameLine();
		ImGui::Checkbox("##Show Bvh Volume", &WireframeRenderer::ShowBvhBoxes);
	}
}