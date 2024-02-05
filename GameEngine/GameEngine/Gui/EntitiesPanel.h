#pragma once
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>
#include <string>

#define TITLE_EP(title) (std::string(title) + "##EntitiesPanel").c_str()

class EntitiesPanel
{
private:
	static void Render();
	static void Update();
	friend class Gui;
};

