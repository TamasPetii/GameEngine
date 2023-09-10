#include "TestScript.h"

void TestScript::OnStart()
{

}

void TestScript::OnUpdate(float deltaTime)
{
	static float currentTime = 0;
	currentTime += deltaTime;

	TransformComponent* transformComponent = m_Entity->GetComponent<TransformComponent>();
	transformComponent->Ref_Translation() = glm::vec3(10 * cosf(currentTime), 13 + 2 * cosf(currentTime), 10 * sinf(currentTime));
	transformComponent->Ref_Rotation() = glm::vec3(90 * cosf(currentTime), 90 * cosf(sinf(currentTime)), 90 * sinf(currentTime));
	transformComponent->Ref_Scale() = glm::vec3(2 * abs(cosf(sinf(currentTime))), 2 * abs(sinf(currentTime)), 2 * abs(cosf(currentTime)));

	MeshComponent* meshComponent = m_Entity->GetComponent<MeshComponent>();
	meshComponent->Ref_Material().diffuse = glm::vec3(cosf(currentTime), sinf(currentTime), cosf(sinf(currentTime))) * 0.5f + glm::vec3(0.5f);
}
