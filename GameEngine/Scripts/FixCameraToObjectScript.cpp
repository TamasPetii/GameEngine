#include "FixCameraToObjectScript.h"

void FixCameraToObjectScript::OnStart()
{
}

void FixCameraToObjectScript::OnUpdate(float deltaTime)
{
	static float time = 0;
	auto cameraPool = registry->GetComponentPool<CameraComponent>();
	auto defaultColliderPool = registry->GetComponentPool<DefaultCollider>();
	auto transformPool = registry->GetComponentPool<TransformComponent>();

	if (!cameraPool || !defaultColliderPool ||!transformPool)
		return;

	bool isFinished = transformPool->HasComponent(entity) && transformPool->GetComponent(entity).translate.z > 315;

	auto& defaultCollider = defaultColliderPool->GetComponent(entity);
	auto& cameraComponent = CameraSystem::GetMainCamera(registry);
	cameraComponent.enableUserMovement = false;

	if (!isFinished)
	{
		glm::vec3 dir = glm::normalize(glm::vec3(0, 1.2, -1));
		cameraComponent.position = defaultCollider.origin + dir * 50.f;
		cameraComponent.target = defaultCollider.origin;
	}
	else
	{
		time += deltaTime;
		float speed = 0.1f;
		glm::vec3 dir = glm::vec3(sinf(time * 2 * M_PI * speed), 1, cosf(time * 2 * M_PI * speed));
		cameraComponent.position = defaultCollider.origin + dir * 25.f;
		cameraComponent.target = defaultCollider.origin;
	}
}
