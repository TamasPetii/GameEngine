#include "ChangeAnimationScript.h"

ChangeAnimationScript::~ChangeAnimationScript()
{
	if (soundBg)
	{
		soundBg->stop();
	}

	if (sound)
	{
		sound->stop();
	}
}

void ChangeAnimationScript::OnStart()
{
}

void ChangeAnimationScript::OnUpdate(float deltaTime)
{
	auto inputManager = InputManager::Instance();
	auto animationPool = registry->GetComponentPool<AnimationComponent>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();

	if (!animationPool || !transformPool)
		return;

	auto& animationComponent = animationPool->GetComponent(entity);
	bool isJumping = inputManager->IsKeyHeld(GLFW_KEY_SPACE);
	bool isMoving = inputManager->IsKeyHeld(GLFW_KEY_W) || inputManager->IsKeyHeld(GLFW_KEY_S) || inputManager->IsKeyHeld(GLFW_KEY_A) || inputManager->IsKeyHeld(GLFW_KEY_D);
	bool isSprinting = isMoving && inputManager->IsKeyHeld(GLFW_KEY_LEFT_SHIFT);
	bool isFinished = transformPool->HasComponent(entity) && transformPool->GetComponent(entity).translate.z > 315;

	if (soundBg == nullptr)
	{
		auto soundSource = SoundManager::Instance()->LoadSoundSource("../Assets/Bg.mp3");
		soundBg = SoundManager::Instance()->PlaySound2D(soundSource, true);
		soundBg->setVolume(0.5f);
	}

	if (isFinished)
	{
		static std::string jumpingName = "../Assets/Animation/Character_Dance.dae";

		if (animationComponent.animation && animationComponent.animation->GetPath() != jumpingName)
		{
			animationComponent.time = 0;
			animationComponent.animation = ModelManager::Instance()->LoadAnimation(jumpingName);

			if (soundBg)
			{
				soundBg->stop();
			}

			if (sound)
			{
				sound->stop();
				sound = nullptr;
			}

			auto soundSource = SoundManager::Instance()->LoadSoundSource("../Assets/Finish.mp3");
			sound = SoundManager::Instance()->PlaySound2D(soundSource, true);
		}
	}
	else if (isJumping)
	{
		static std::string jumpingName = "../Assets/Animation/Character_Jumping.dae";

		if (animationComponent.animation && animationComponent.animation->GetPath() != jumpingName)
		{
			animationComponent.time = 0;
			animationComponent.animation = ModelManager::Instance()->LoadAnimation(jumpingName);

			if (sound)
			{
				sound->stop();
				sound = nullptr;
			}
		}
	}
	else if (isSprinting)
	{
		static std::string sprintingName = "../Assets/Animation/Character_Running.dae";

		if (animationComponent.animation && animationComponent.animation->GetPath() != sprintingName)
		{
			animationComponent.time = 0;
			animationComponent.animation = ModelManager::Instance()->LoadAnimation(sprintingName);

			if (sound)
			{
				sound->stop();
				sound = nullptr;
			}

			auto soundSource = SoundManager::Instance()->LoadSoundSource("../Assets/Running.mp3");
			sound = SoundManager::Instance()->PlaySound2D(soundSource, true);
		}
	}
	else if (isMoving)
	{
		static std::string walkingName = "../Assets/Animation/Character_Walking.dae";

		if (animationComponent.animation && animationComponent.animation->GetPath() != walkingName)
		{
			animationComponent.time = 0;
			animationComponent.animation = ModelManager::Instance()->LoadAnimation(walkingName);

			if (sound)
			{
				sound->stop();
				sound = nullptr;
			}

			auto soundSource = SoundManager::Instance()->LoadSoundSource("../Assets/Walking.mp3");
			sound = SoundManager::Instance()->PlaySound2D(soundSource, true);
		}
	}
	else
	{
		static std::string standingName = "../Assets/Animation/Character_Standing.dae";

		if (animationComponent.animation && animationComponent.animation->GetPath() != standingName)
		{
			animationComponent.time = 0;
			animationComponent.animation = ModelManager::Instance()->LoadAnimation(standingName);

			if (sound)
			{
				sound->stop();
				sound = nullptr;
			}
		}
	}
}
