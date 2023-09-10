#include "LightMoveScript.h"

void LightMoveScript::OnStart()
{
}

void LightMoveScript::OnUpdate(float deltaTime)
{
	float radius = 8.f;
	float speed = 2.5f;

	static float currentTime = 0;
	currentTime += deltaTime;

	TransformComponent* transformComponent = m_Entity->GetComponent<TransformComponent>();
	transformComponent->Ref_Translation() = glm::vec3(radius * cosf(currentTime), 5, radius * sinf(currentTime));

	auto lightComponent = m_Entity->GetComponent<LightComponent>();
	lightComponent->Ref_LightSource()->Ref_DiffuseIntensity() = abs(cosf(currentTime));
	lightComponent->Ref_LightSource()->Ref_Color() = glm::vec3(abs(cosf(sinf(currentTime))), abs(cosf(cosf(currentTime))), abs(sinf(sinf(currentTime))));
}
