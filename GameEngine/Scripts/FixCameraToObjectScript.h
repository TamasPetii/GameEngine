#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include "BaseScript.h"
#include "Manager/InputManager.h"

class SCRIPT_API FixCameraToObjectScript : public BaseScript
{
public:
	FixCameraToObjectScript(std::shared_ptr<Registry> registry, Entity entity) : BaseScript(registry, entity) {}
	void OnStart() override;
	void OnUpdate(float deltaTime) override;
};

extern "C" SCRIPT_API BaseScript* CreateScript_FixCameraToObjectScript(std::shared_ptr<Registry> registry, Entity entity)
{
	return new FixCameraToObjectScript(registry, entity);
}
