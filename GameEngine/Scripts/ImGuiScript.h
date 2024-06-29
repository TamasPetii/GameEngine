#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include "BaseScript.h"
#include "ImGui_Glfw/imgui.h"


class DLL_API ImGuiScript : public BaseScript
{
public:
	ImGuiScript(std::shared_ptr<Registry> registry, Entity entity) : BaseScript(registry, entity) {}
	void OnStart() override;
	void OnUpdate(float deltaTime) override;
	void OnGui() override;
};

extern "C" DLL_API BaseScript * CreateScript_ImGuiScript(std::shared_ptr<Registry> registry, Entity entity)
{
	return new ImGuiScript(registry, entity);
}
