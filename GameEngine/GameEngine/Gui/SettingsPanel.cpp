#include "SettingsPanel.h"

void SettingsPanel::Update()
{

}

void SettingsPanel::Render(std::shared_ptr<ResourceManager> resourceManager)
{
	if (ImGui::Begin(TITLE_SP("SettingsPanel")))
	{
		RenderTextures(resourceManager);
		RenderBloomTextures(resourceManager);
	}

	ImGui::End();
}

void SettingsPanel::RenderTextures(std::shared_ptr<ResourceManager> resourceManager)
{
    if (ImGui::CollapsingHeader(TITLE_SP("Framebuffer Textures")))
    {
		auto fbo = resourceManager->GetFbo("Main");
		ImGui::Image((ImTextureID)fbo->GetTextureID("color"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("normal"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("bloom"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)fbo->GetTextureID("main"), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
    }
}

void SettingsPanel::RenderBloomTextures(std::shared_ptr<ResourceManager> resourceManager)
{
	if (ImGui::CollapsingHeader(TITLE_SP("Bloom Textures")))
	{
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