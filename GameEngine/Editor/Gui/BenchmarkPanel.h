#pragma once
#include <string>
#include <memory>
#include "Scene/Scene.h"

#define TITLE_BP(title) (std::string(title) + "##BenchmarkPanel").c_str()

class BenchmarkPanel
{
private:
	static void Render(std::shared_ptr<Scene> scene, std::unordered_map<std::type_index, double>& averagePanelTime);
	static void RenderGeometryStats();
	static void RenderSystemTimes(std::shared_ptr<Scene> scene);
	static void RenderRenderTimes();
	static void RenderGuiTimes(std::unordered_map<std::type_index, double>& averagePanelTime);
	friend class Gui;
};

