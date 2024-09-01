#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <string>
#include <memory>
#include <iostream>
#include <GL/glew.h>
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>
#include <ImGui_Glfw/ImGuizmo.h>
#include <GLM/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>
#include "Scene/Scene.h"
#include "Manager/ResourceManager.h"
#include "Settings/GlobalSettings.h"

#define TITLE_VP(title) (std::string(title) + "##ViewportPanel").c_str()

class ViewportPanel
{
private:
	static void Render(std::shared_ptr<Scene> scene, float deltaTime);
	static void Update(std::shared_ptr<Scene> scene);
	static void CameraButtonEvent(std::shared_ptr<Scene> scene);
	static void CameraKeyboardEvent(std::shared_ptr<Scene> scene);
	static void RenderGizmos(std::shared_ptr<Scene> scene);
	static void RenderFpsCounter(std::shared_ptr<Scene> scene, float deltaTime);
	friend class Gui;
private:
	static inline ImVec2 m_ViewportSize;
	static inline bool m_ViewportSizeChanged{ false };
};

