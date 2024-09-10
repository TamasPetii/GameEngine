#pragma once
#include <filesystem>
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
#include <Gui/ConsolePanel.h>
#include "Settings/GlobalSettings.h"

class Gui
{
public:
	static void SetStyle();
	static void Render(std::shared_ptr<Scene> scene, float deltaTime);
	static void Update(std::shared_ptr<Scene> scene);
private:
	static void PreRender();
	static void PostRender();
	static void RenderDockSpace(std::shared_ptr<Scene> scene);
	static void RenderScriptGui(std::shared_ptr<Scene> scene);
	static void RenderMainTitleBar(std::shared_ptr<Scene> scene);
	static void RenderPopupModals(std::shared_ptr<Scene> scene);

	static bool OpenGlobalSettingsPopup;
	static void ShowGlobalSettingsPopup();
};

