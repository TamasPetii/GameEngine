#include "ChangeAnimationScript.h"

void ChangeAnimationScript::OnStart()
{
}

void ChangeAnimationScript::OnUpdate(float deltaTime)
{
	static float currentTime = 0;
	currentTime += deltaTime;

	static bool changeAnimation = false;

	if (registry->HasComponent<AnimationComponent>(entity))
	{
		/*
		if (currentTime > 5)
		{
			currentTime = 0;
			changeAnimation = !changeAnimation;

			auto& animationComponent = registry->GetComponent<AnimationComponent>(entity);
			animationComponent.time = 0;

			if(changeAnimation)
				animationComponent.animation = ModelManager::Instance()->LoadAnimation("../Assets/Models/Mixamo/Character - Walking/CharacterWalking.dae");
			else
				animationComponent.animation = ModelManager::Instance()->LoadAnimation("../Assets/Models/Mixamo/Character - Jumping/CharacterJumping.dae");
		}
		*/

	}

	registry->SetFlag<AnimationComponent>(entity, CHANGED_FLAG);
	registry->SetFlag<AnimationComponent>(entity, REGENERATE_FLAG);
}
