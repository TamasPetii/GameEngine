#include "MoveEntityScript.h"

void MoveEntityScript::OnStart()
{
}

void MoveEntityScript::OnUpdate(float deltaTime)
{
	/*
	static float currentTime = 0;
	currentTime += deltaTime;

	auto& transformComponent = registry->GetComponent<TransformComponent>(entity);
	transformComponent.translate += glm::vec3(0.1, 0, 0);

	registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
	std::cout << asd << std::endl;
	std::cout << ModelManager::Instance()->GetModelsList().size() << std::endl;
	std::cout << "MoveEntityScript executed!" << std::endl;
	std::cout << "Transform Translate = " << transformComponent.translate.x << " " << transformComponent.translate.y << " " << transformComponent.translate.z << std::endl;
	*/
}
