#pragma once
#include "ScriptApi.h"

#include <iostream>
#include <glm/glm.hpp>
#include "Manager/InputManager.h"

class SCRIPT_API MoveEntityScript : public Script
{
public:
	MoveEntityScript(std::shared_ptr<Registry> registry, Entity entity) : Script(registry, entity) {}
	void OnStart() override;
	void OnUpdate(float deltaTime) override;
};

extern "C" SCRIPT_API Script* CreateScript_MoveEntityScript(std::shared_ptr<Registry> registry, Entity entity)
{
	return new MoveEntityScript(registry, entity);
}
