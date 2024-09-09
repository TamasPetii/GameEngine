#include "ChangeAnimationScript.h"

void ChangeAnimationScript::OnStart()
{
}

void ChangeAnimationScript::OnUpdate(float deltaTime)
{
	auto inputManager = InputManager::Instance();
	auto animationPool = registry->GetComponentPool<AnimationComponent>();

	if (!animationPool)
		return;

	auto& animationComponent = animationPool->GetComponent(entity);

	bool isJumping = inputManager->IsKeyHeld(GLFW_KEY_SPACE);
	bool isMoving = inputManager->IsKeyHeld(GLFW_KEY_W) || inputManager->IsKeyHeld(GLFW_KEY_S) || inputManager->IsKeyHeld(GLFW_KEY_A) || inputManager->IsKeyHeld(GLFW_KEY_D);
	bool isSprinting = isMoving && inputManager->IsKeyHeld(GLFW_KEY_LEFT_SHIFT);



	if (isJumping)
	{
		static std::string jumpingName = "C:/Users/User/Desktop/GameEngine/GameEngine/Assets/Animation/Character_Jumping.dae";

		if (animationComponent.animation && animationComponent.animation->GetPath() != jumpingName)
		{
			animationComponent.time = 0;
			animationComponent.animation = ModelManager::Instance()->LoadAnimation(jumpingName);
		}
	}
	else if (isSprinting)
	{
		static std::string sprintingName = "C:/Users/User/Desktop/GameEngine/GameEngine/Assets/Animation/Character_Running.dae";

		if (animationComponent.animation && animationComponent.animation->GetPath() != sprintingName)
		{
			animationComponent.time = 0;
			animationComponent.animation = ModelManager::Instance()->LoadAnimation(sprintingName);
		}
	}
	else if (isMoving)
	{
		static std::string walkingName = "C:/Users/User/Desktop/GameEngine/GameEngine/Assets/Animation/Character_Walking.dae";

		if (animationComponent.animation && animationComponent.animation->GetPath() != walkingName)
		{
			animationComponent.time = 0;
			animationComponent.animation = ModelManager::Instance()->LoadAnimation(walkingName);
		}
	}
	else
	{
		static std::string standingName = "C:/Users/User/Desktop/GameEngine/GameEngine/Assets/Animation/Character_Standing.dae";

		if (animationComponent.animation && animationComponent.animation->GetPath() != standingName)
		{
			animationComponent.time = 0;
			animationComponent.animation = ModelManager::Instance()->LoadAnimation(standingName);
		}
	}
}
