#pragma once
#include "ScriptApi.h"

#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include "Manager/InputManager.h"

class SCRIPT_API MazeScript : public Script
{
public:
	MazeScript(std::shared_ptr<Registry> registry, Entity entity) : Script(registry, entity) {}
	void OnStart() override;
	void OnUpdate(float deltaTime) override;

	std::vector<std::vector<bool>> maze;
};

extern "C" SCRIPT_API Script* CreateScript_MazeScript(std::shared_ptr<Registry> registry, Entity entity)
{
	return new MazeScript(registry, entity);
}
