#include "MoveEntityScript.h"

void MoveEntityScript::OnStart()
{
}

void MoveEntityScript::OnUpdate(float deltaTime)
{
	static float currentTime = 0;
	currentTime += deltaTime;

	auto& transformComponent = registry->GetComponent<TransformComponent>(entity);
	if (transformComponent.translate.y < 0)
		transformComponent.translate.y = 25;

	registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);

	std::cout << CollisionSystem::aabbCounter.load() << std::endl;
	std::cout << ModelManager::Instance()->GetModelsList().size() << std::endl;
}
