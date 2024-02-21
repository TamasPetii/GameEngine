#pragma once
<<<<<<< HEAD
#include <string>
#include <memory>
#include <GL/glew.h>
=======
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>
<<<<<<< HEAD
#include <Manager/ResourceManager.h>
#include "Render/BloomRenderer.h"
#include "Registry/System/BvhSystem.h"
#include "Registry/System/CollisionSystem.h"
#include "Scene/Scene.h"
=======
#include <string>
#include <memory>
#include <Manager/ResourceManager.h>
#include <Render/BloomRenderer.h>
#include <Registry/System/BvhSystem.h>
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

#define TITLE_SP(title) (std::string(title) + "##SettingsPanel").c_str()

class SettingsPanel
{
private:
<<<<<<< HEAD
	static void Render(std::shared_ptr<Scene> scene);
	static void Update(std::shared_ptr<Scene> scene);
	static void RenderTextures();
	static void RenderBloomTextures();
	static void RenderCollisionStats();
	static void RenderSystemTimes(std::shared_ptr<Scene> scene);
=======
	static void Render(std::shared_ptr<ResourceManager> resourceManager);
	static void Update();
	static void RenderBvh(std::shared_ptr<ResourceManager> resourceManager);
	static void RenderTextures(std::shared_ptr<ResourceManager> resourceManager);
	static void RenderBloomTextures(std::shared_ptr<ResourceManager> resourceManager);
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	friend class Gui;
};

