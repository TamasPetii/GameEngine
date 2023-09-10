#include "MoveEntityScript.h"

void MoveEntityScript::OnStart()
{
}

void MoveEntityScript::OnUpdate(float deltaTime)
{
	static float currentTime = 0;
	currentTime += deltaTime;

	TransformComponent* transformComponent = m_Entity->GetComponent<TransformComponent>();
	transformComponent->Ref_Translation() = glm::vec3(0, 5 * abs(cosf(currentTime)), 0);
}
