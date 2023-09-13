#include "DynamicColor.h"

void DynamicColor::OnStart()
{
}

void DynamicColor::OnUpdate(float deltaTime)
{
	static float currentTime = 0;
	currentTime += deltaTime;

	if (m_Entity->HasComponent<MeshComponent>())
	{
		m_Entity->GetComponent<MeshComponent>()->Ref_Material().diffuse = glm::vec3(sinf(currentTime), cosf(currentTime), sinf(currentTime) * cosf(currentTime)) * 0.5f + glm::vec3(0.5f);
	}
}
