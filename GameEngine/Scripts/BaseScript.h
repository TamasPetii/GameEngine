#pragma once
#pragma warning(disable: 4251)
#define _USE_MATH_DEFINES 
#include <cmath>
#include <iostream>
#include <filesystem>
#include <memory>
#include "Registry/Registry.h"
#include "Registry/Component/Components.h"
#include "ImGui_Glfw/imgui.h"

#ifdef EXPORT_SCRIPT_DLL
#define SCRIPT_API __declspec(dllexport)
#else
#define SCRIPT_API __declspec(dllimport)
#endif

class SCRIPT_API BaseScript
{
public:
	BaseScript(std::shared_ptr<Registry> registry, Entity entity) : registry(registry), entity(entity) {}
	virtual ~BaseScript() = default;
	virtual void OnStart() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnGui() {}
	virtual void SetImGuiContext(ImGuiContext* context) { if(ImGui::GetCurrentContext() != context) ImGui::SetCurrentContext(context); }
protected:
	Entity entity;
	std::shared_ptr<Registry> registry;
};

