#pragma once
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>
#include <string>
#include "FileDialogWindows.h"

#define TITLE_CP(title) (std::string(title) + "##ComponentPanel").c_str()

#include <Render/Renderer.h>

class ComponentPanel
{
private:
	static void Render();
	static void Update();
	static void RenderTransformComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderMaterialComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderDirlightComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderPointLightComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderSpotLightComponent(std::shared_ptr<Registry> registry, Entity entity);
	friend class Gui;
};

