#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>

#include <Gui/ViewportPanel.h>
#include <Gui/EntitiesPanel.h>
#include <Gui/ComponentPanel.h>
#include <Gui/FilesystemPanel.h>
#include <Gui/SettingsPanel.h>

class Gui
{
public:
	static void SetStyle();
	static void Render();
	static void Update();
private:
	static void PreRender();
	static void PostRender();
	static void RenderDockSpace();
};

