#pragma once
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>
#include <string>
#include <memory>
#include <Manager/ResourceManager.h>
#include <Render/BloomRenderer.h>
#include <Registry/System/BvhSystem.h>

#define TITLE_SP(title) (std::string(title) + "##SettingsPanel").c_str()

class SettingsPanel
{
private:
	static void Render(std::shared_ptr<ResourceManager> resourceManager);
	static void Update();
	static void RenderBvh(std::shared_ptr<ResourceManager> resourceManager);
	static void RenderTextures(std::shared_ptr<ResourceManager> resourceManager);
	static void RenderBloomTextures(std::shared_ptr<ResourceManager> resourceManager);
	friend class Gui;
};

