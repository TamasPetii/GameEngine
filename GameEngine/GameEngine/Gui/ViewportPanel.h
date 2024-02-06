#pragma once
#include <GL/glew.h>
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>
#include <Render/OpenGL/Camera.h>
#include <Render/Renderer.h>
#include <string>
#include <memory>
#include <iostream>

#define TITLE_VP(title) (std::string(title) + "##ViewportPanel").c_str()

class ViewportPanel
{
private:
	static void Render();
	static void Update();
	static void CameraButtonEvent(std::shared_ptr<Camera> camera);
	static void CameraKeyboardEvent(std::shared_ptr<Camera> camera);
	friend class Gui;
private:
	static inline ImVec2 m_ViewportSize;
	static inline bool m_ViewportSizeChanged{ false };
};

