#pragma once
#include <string>
#include <GL/glew.h>
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>
<<<<<<< HEAD
#include "FileDialogWindows.h"
#include "Scene/Scene.h"
#include <filesystem>
#include "IconsFontAwesome5.h"
=======
#include <string>
#include "FileDialogWindows.h"

>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
#define TITLE_CP(title) (std::string(title) + "##ComponentPanel").c_str()

class ComponentPanel
{
private:
<<<<<<< HEAD
	static void Render(std::shared_ptr<Scene> scene);
	static void Update(std::shared_ptr<Scene> scene);
	static void RenderTagComponent(std::shared_ptr<Registry> registry, Entity entity);
=======
	static void Render();
	static void Update();
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	static void RenderTransformComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderMaterialComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderDirlightComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderPointLightComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderSpotLightComponent(std::shared_ptr<Registry> registry, Entity entity);
<<<<<<< HEAD
	static void RenderAddComponentPopUp(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderShapeComponent(std::shared_ptr<Registry> registry, Entity entity);
	static std::pair<bool, std::shared_ptr<TextureGL>> TextureAssetPopup();
=======
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	friend class Gui;
};

