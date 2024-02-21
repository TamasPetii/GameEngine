#pragma once
#include <string>
#include <filesystem>
#include <GL/glew.h>
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>
<<<<<<< HEAD
#include "Manager/TextureManager.h"
#include <deque>
#include <glm/glm.hpp>
#include "IconsFontAwesome5.h"
=======
#include <string>
#include <filesystem>

>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
#define TITLE_FP(title) (std::string(title) + "##FilesystemPanel").c_str()

class FilesystemPanel
{
private:
	static void Render();
	static void Update();
	static void RenderFileSystem();
	static void RenderFolderSystem(const std::string& folderPath = "../Assets");
	friend class Gui;
};

