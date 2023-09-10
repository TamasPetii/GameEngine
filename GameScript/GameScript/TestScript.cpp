#include "TestScript.h"

void TestScript::OnStart()
{

}

void TestScript::OnUpdate(float deltaTime)
{
	static float currentTime = 0;
	currentTime += deltaTime * 2;

	TransformComponent* transformComponent = m_Entity->GetComponent<TransformComponent>();
	transformComponent->Ref_Translation() = glm::vec3(10 * cosf(currentTime), 0, 10 * sinf(currentTime));
	transformComponent->Ref_Rotation() = glm::vec3(90 * cosf(currentTime), 90 * cosf(sinf(currentTime)), 90 * sinf(currentTime));
	transformComponent->Ref_Scale() = glm::vec3(5 * abs(cosf(sinf(currentTime))), 5 * abs(sinf(currentTime)), 5 * abs(cosf(currentTime)));

	MeshComponent* meshComponent = m_Entity->GetComponent<MeshComponent>();
	meshComponent->Ref_Material().diffuse = glm::vec3(cosf(currentTime), sinf(currentTime), cosf(sinf(currentTime))) * 0.5f + glm::vec3(0.5f);

	//std::cout << "TransformComponent = " << "(" << transformComponent->Ref_Translation().x << "," << transformComponent->Ref_Translation().y << "," << transformComponent->Ref_Translation().z << ")" << std::endl;
}
