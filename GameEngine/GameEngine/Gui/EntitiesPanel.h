#pragma once
#include <string>
#include <GL/glew.h>
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>
<<<<<<< HEAD
#include "Scene/Scene.h"
#include "IconsFontAwesome5.h"
=======
#include <string>
#include <Render/Renderer.h>
#include <Registry/Component/DirlightComponent.h>

>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
#define TITLE_EP(title) (std::string(title) + "##EntitiesPanel").c_str()

class EntitiesPanel
{
private:
<<<<<<< HEAD
	static void Render(std::shared_ptr<Scene> scene);
	static void Update(std::shared_ptr<Scene> scene);
	static void DisplayEntity(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderAddEntityPopUp(std::shared_ptr<Registry> registry);
	static inline float regionWidth = 0;
=======
	static void Render();
	static void Update();
	static void DisplayEntity(std::shared_ptr<Registry> registry, Entity entity);
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	friend class Gui;
};

