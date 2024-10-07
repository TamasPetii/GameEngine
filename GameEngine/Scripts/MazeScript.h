#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include "BaseScript.h"
#include "Manager/InputManager.h"

class SCRIPT_API MazeScript : public BaseScript
{
public:
	MazeScript(std::shared_ptr<Registry> registry, Entity entity) : BaseScript(registry, entity) {}
	void OnStart() override;
	void OnUpdate(float deltaTime) override;

	std::vector<std::vector<bool>> maze;
};

extern "C" SCRIPT_API BaseScript* CreateScript_MazeScript(std::shared_ptr<Registry> registry, Entity entity)
{
	return new MazeScript(registry, entity);
}
