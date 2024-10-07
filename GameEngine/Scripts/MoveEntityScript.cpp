#include "MoveEntityScript.h"

void MoveEntityScript::OnStart()
{
}

void MoveEntityScript::OnUpdate(float deltaTime)
{
	auto transformPool = registry->GetComponentPool<TransformComponent>();

	if (!transformPool)
		return;

	auto& transformComponent = transformPool->GetComponent(entity);

	float speed = 30;

	if (InputManager::Instance()->IsKeyHeld(GLFW_KEY_LEFT_SHIFT))
		speed = 50;

	if (InputManager::Instance()->IsKeyHeld(GLFW_KEY_W))
	{
		transformComponent.translate.z += speed * deltaTime;
		transformPool->SetFlag(entity, UPDATE_FLAG);
	}

	if (InputManager::Instance()->IsKeyHeld(GLFW_KEY_S))
	{
		transformComponent.translate.z -= speed * deltaTime;
		transformPool->SetFlag(entity, UPDATE_FLAG);
	}

	if (InputManager::Instance()->IsKeyHeld(GLFW_KEY_D))
	{
		transformComponent.translate.x -= speed * deltaTime;
		transformPool->SetFlag(entity, UPDATE_FLAG);
	}

	if (InputManager::Instance()->IsKeyHeld(GLFW_KEY_A))
	{
		transformComponent.translate.x += speed * deltaTime;
		transformPool->SetFlag(entity, UPDATE_FLAG);
	}

	transformComponent.translate.y = -10;
	transformPool->SetFlag(entity, UPDATE_FLAG);
}
