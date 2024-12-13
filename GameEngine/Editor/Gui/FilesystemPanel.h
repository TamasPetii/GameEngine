#pragma once
#include <string>
#include <filesystem>
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>
#include "IconsFontAwesome5.h"


#define TITLE_FP(title) (std::string(title) + "##FilesystemPanel").c_str()

class FilesystemPanel
{
private:
	static void Render();
	static void Update();
	static void RenderFileSystem();
	static void RenderFolderSystem(const std::string& folderPath);
	static bool EnablePreview;
	static std::filesystem::path fileSystemPath;
	friend class Gui;
};

