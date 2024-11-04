#pragma once
#include "ScriptApi.h"

#include <iostream>
#include <glm/glm.hpp>
#include "Manager/InputManager.h"

class SCRIPT_API FixCameraToObjectScript : public Script
{
public:
	FixCameraToObjectScript(std::shared_ptr<Registry> registry, Entity entity) : Script(registry, entity) {}
	void OnStart() override;
	void OnUpdate(float deltaTime) override;
};

extern "C" SCRIPT_API Script* CreateScript_FixCameraToObjectScript(std::shared_ptr<Registry> registry, Entity entity)
{
	return new FixCameraToObjectScript(registry, entity);
}
