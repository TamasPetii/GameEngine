#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <string>
#include <memory>
#include <ImGui_Glfw/imgui.h>
#include "Scene/Scene.h"

#define TITLE_VP(title) (std::string(title) + "##ViewportPanel").c_str()

class ViewportPanel
{
private:
	static void Render(std::shared_ptr<Scene> scene, float deltaTime);
	static void Update(std::shared_ptr<Scene> scene);
	static void RenderGizmos(std::shared_ptr<Scene> scene);
	static void RenderFpsCounter(std::shared_ptr<Scene> scene, float deltaTime);
	friend class Gui;
private:
	static ImVec2 m_ViewportSize;
	static bool m_ViewportSizeChanged;
};

