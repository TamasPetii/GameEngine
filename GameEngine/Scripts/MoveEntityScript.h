#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include "BaseScript.h"
#include "Manager/ModelManager.h"
#include "Registry/System/CollisionSystem.h"

class DLL_API MoveEntityScript : public BaseScript
{
public:
	MoveEntityScript(std::shared_ptr<Registry> registry, Entity entity) : BaseScript(registry, entity) {}
	void OnStart() override;
	void OnUpdate(float deltaTime) override;
};

extern "C" DLL_API BaseScript * CreateScript_MoveEntityScript(std::shared_ptr<Registry> registry, Entity entity)
{
	return new MoveEntityScript(registry, entity);
}
