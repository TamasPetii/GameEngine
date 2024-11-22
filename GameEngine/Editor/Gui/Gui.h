#pragma once
#include <filesystem>
#include <nlohmann/json.hpp>
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
#include <fstream>

#include "tinyxml2.h"

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

	static bool OpenAskSceneSavePopup;
	static void ShowAskSceneSavePopup(std::shared_ptr<Scene> scene);

	static bool OpenNewProjectPopup;
	static void ShowNewProjectPopup(std::shared_ptr<Scene> scene);
	static bool GenerateProject(const std::string& parentPath, const std::string& name);
	static bool CopyFolderAndContent(const std::string& sourceFolderPath, const std::string& folderPath, const std::string projectPath);

	static bool OpenLoadProjectPopup;
	static void ShowLoadProjectPopup(std::shared_ptr<Scene> scene);

	static bool OpenInitialPopup;
	static void ShowInitialPopup();
	static bool CheckAndLoadInitData();
	static void GenerateInitData(const std::string& defaultPath, const std::string& compilerPath);

	static bool OpenInitProjectPopup;
	static void ShowInitProjectPopup();

	static bool OpenBuildProjectPopup;
	static void BuildProjectToDeployedGame(std::shared_ptr<Scene> scene);
	static bool CheckIfDeployedGame(std::shared_ptr<Scene> scene);
	static bool GenerateBuildGameProject(std::shared_ptr<Scene> scene, const std::string& parentPath, const std::string& name);

	static bool UpdateScriptVcxprojPaths();
};

