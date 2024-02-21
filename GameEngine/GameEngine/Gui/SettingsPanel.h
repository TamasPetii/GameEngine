#pragma once
#include <string>
#include <memory>
#include <GL/glew.h>
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>
#include <Manager/ResourceManager.h>
#include "Render/BloomRenderer.h"
#include "Registry/System/BvhSystem.h"
#include "Registry/System/CollisionSystem.h"
#include "Scene/Scene.h"

#define TITLE_SP(title) (std::string(title) + "##SettingsPanel").c_str()

class SettingsPanel
{
private:
	static void Render(std::shared_ptr<Scene> scene);
	static void Update(std::shared_ptr<Scene> scene);
	static void RenderTextures();
	static void RenderBloomTextures();
	static void RenderCollisionStats();
	static void RenderSystemTimes(std::shared_ptr<Scene> scene);
	friend class Gui;
};

