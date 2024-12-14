#include "SettingsPanel.h"
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>

#include "Manager/ResourceManager.h"
#include "Settings/GlobalSettings.h"
#include "Registry/System/Systems.h"
#include "Benchmark/BenchmarkManager.h"

#include "Render/BloomRenderer.h"
#include "Render/BillboardRenderer.h"
#include "Render/WireframeRenderer.h"

#include "Render/OpenGL/TextureGL.h"
#include "Render/OpenGL/FramebufferGL.h"


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
		RenderWireframeSettings();
	}

	ImGui::End();
}

void SettingsPanel::RenderTextures(std::shared_ptr<Scene> scene)
{
    if (ImGui::CollapsingHeader(TITLE_SP("Framebuffer Textures")))
    {
		//ImGui::DragFloat("Bias", &GlobalSettings::bias, 0.001f);

		auto resourceManager = ResourceManager::Instance();
		auto fbo = resourceManager->GetFbo("Main");
		ImGui::Image((ImTextureID)fbo->GetTextureID("color"),  ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("normal"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("position"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("bloom"),  ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("main"),   ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
    }
}

void SettingsPanel::RenderBloomTextures()
{
	if (ImGui::CollapsingHeader(TITLE_SP("Bloom Textures")))
	{
		auto resourceManager = ResourceManager::Instance();
		auto fbo = resourceManager->GetFbo("Bloom");
		ImGui::Checkbox(TITLE_SP("Use Bloom"), &BloomRenderer::useBloom);
		ImGui::DragFloat(TITLE_SP("Gamma"),    &BloomRenderer::gamma, 0.005f, 0.f, 100.f);
		ImGui::DragFloat(TITLE_SP("Exposure"), &BloomRenderer::exposure, 0.005f, 0.f, 100.f);
		ImGui::Image((ImTextureID)fbo->GetTextureID("bloom0"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("bloom1"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("bloom2"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("bloom3"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("bloom4"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("bloom5"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
	}
}

void SettingsPanel::RenderWireframeSettings()
{
	if (ImGui::CollapsingHeader(TITLE_SP("Wireframe Settings")))
	{
		ImGui::Text("Icon Size");
		ImGui::SameLine();
		ImGui::DragFloat("##Icon Size", &BillboardRenderer::iconSize, 0.005, 0.f, 10.f);

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

		ImGui::Text("Show Box Collider Volume");
		ImGui::SameLine();
		ImGui::Checkbox("##Show box Collider Volume", &WireframeRenderer::ShowBoxCollider);

		ImGui::Text("Show Sphere Collider Volume");
		ImGui::SameLine();
		ImGui::Checkbox("##Show Sphere Collider Volume", &WireframeRenderer::ShowSphereCollider);

		ImGui::Text("Show Camera Volume");
		ImGui::SameLine();
		ImGui::Checkbox("##Show Camera Volume", &WireframeRenderer::ShowCameraVolume);
	}
}