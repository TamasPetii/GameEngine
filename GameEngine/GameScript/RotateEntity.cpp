#include "RotateEntity.h"

void RotateEntity::OnStart()
{
}

void RotateEntity::OnUpdate(float deltaTime)
{
	static float currentTime = 0;
	currentTime += deltaTime;

	auto transform = m_Entity->GetComponent<TransformComponent>();
	transform->Ref_Rotation() = glm::vec3(90 * cosf(currentTime), 90 * sinf(currentTime), 90 * sinf(currentTime) * cosf(currentTime));
}
