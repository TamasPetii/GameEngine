#include "MoveEntityScript.h"

void MoveEntityScript::OnStart()
{
}

void MoveEntityScript::OnUpdate(float deltaTime)
{
	static float currentTime = 0;
	currentTime += deltaTime;

	std::cout << InputManager::Instance()->IsKeyHeld(GLFW_KEY_W) << std::endl;
}
