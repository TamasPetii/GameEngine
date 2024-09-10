#pragma once
#include <string>
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>
#include "Settings/GlobalSettings.h"
#include "Logger/Logger.h"

class ConsolePanel
{
	static void Render();
	static void Update();
	friend class Gui;
};

