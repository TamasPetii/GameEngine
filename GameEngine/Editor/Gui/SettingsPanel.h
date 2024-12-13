#pragma once
#include <string>
#include <memory>
#include "Scene/Scene.h"

#define TITLE_SP(title) (std::string(title) + "##SettingsPanel").c_str()

class SettingsPanel
{
private:
	static void Render(std::shared_ptr<Scene> scene);
	static void Update(std::shared_ptr<Scene> scene);
	static void RenderTextures(std::shared_ptr<Scene> scene);
	static void RenderBloomTextures();
	static void RenderSystemTimes(std::shared_ptr<Scene> scene);
	static void RenderRenderTimes();
	static void RenderWireframeSettings();
	friend class Gui;
};

