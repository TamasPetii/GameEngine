#pragma once
#include <string>
#include <GL/glew.h>
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>
#include "Scene/Scene.h"
#include "IconsFontAwesome5.h"
#include "Settings/GlobalSettings.h"
#define TITLE_EP(title) (std::string(title) + "##EntitiesPanel").c_str()

class EntitiesPanel
{
private:
	static void Render(std::shared_ptr<Scene> scene);
	static void Update(std::shared_ptr<Scene> scene);
	static void DisplayEntity(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderAddEntityPopUp(std::shared_ptr<Registry> registry);
	static inline float regionWidth = 0;
	friend class Gui;
};

