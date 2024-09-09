#include "FixCameraToObjectScript.h"

void FixCameraToObjectScript::OnStart()
{
}

void FixCameraToObjectScript::OnUpdate(float deltaTime)
{
	auto cameraPool = registry->GetComponentPool<CameraComponent>();
	auto defaultColliderPool = registry->GetComponentPool<DefaultCollider>();

	if (!cameraPool || !defaultColliderPool)
		return;

	auto& defaultCollider = defaultColliderPool->GetComponent(entity);
	auto& cameraComponent = CameraSystem::GetMainCamera(registry);
	cameraComponent.enableUserMovement = false;

	glm::vec3 dir = glm::normalize(glm::vec3(0, 1.2, -1));
	cameraComponent.position = defaultCollider.origin + dir * 50.f;
	cameraComponent.target = defaultCollider.origin;
}
