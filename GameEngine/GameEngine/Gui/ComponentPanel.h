#pragma once
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>
#include <string>

#define TITLE_CP(title) (std::string(title) + "##ComponentPanel").c_str()

#include <Render/Renderer.h>

class ComponentPanel
{
private:
	static void Render();
	static void Update();
	friend class Gui;
};

