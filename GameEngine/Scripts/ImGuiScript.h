#pragma once
#include "ScriptApi.h"

#include <iostream>
#include <glm/glm.hpp>
#include "ImGui_Glfw/imgui.h"


class SCRIPT_API ImGuiScript : public Script
{
public:
	ImGuiScript(std::shared_ptr<Registry> registry, Entity entity) : Script(registry, entity) {}
	void OnStart() override;
	void OnUpdate(float deltaTime) override;
	void OnGui() override;
};

extern "C" SCRIPT_API Script* CreateScript_ImGuiScript(std::shared_ptr<Registry> registry, Entity entity)
{
	return new ImGuiScript(registry, entity);
}
