#include "CameraSystem.h"

void CameraSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void CameraSystem::OnUpdate(std::shared_ptr<Registry> registry, float deltaTime)
{
	auto inputManager = InputManager::Instance();
	auto resourceManager = ResourceManager::Instance();
	auto cameraPool = registry->GetComponentPool<CameraComponent>();

	if (!cameraPool)
		return;

	static CameraGLSL* camDataSsboHandler = nullptr;

	if (!camDataSsboHandler)
	{
		auto camDataSsbo = resourceManager->GetSsbo("CameraData");
		camDataSsboHandler = static_cast<CameraGLSL*>(camDataSsbo->MapBufferRange());
	}

	std::for_each(std::execution::seq, cameraPool->GetDenseEntitiesArray().begin(), cameraPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (true || cameraPool->IsFlagSet(entity, UPDATE_FLAG))
			{
				auto& cameraComponent = cameraPool->GetComponent(entity);
				auto index = cameraPool->GetIndex(entity);
				
				float forward = 0;
				float sideways = 0;

				if (inputManager->IsKeyHeld(GLFW_KEY_W))
					forward = 1;
				if (inputManager->IsKeyHeld(GLFW_KEY_S))
					forward = -1;
				if (inputManager->IsKeyHeld(GLFW_KEY_D))
					sideways = 1;
				if (inputManager->IsKeyHeld(GLFW_KEY_A))
					sideways = -1;

				cameraComponent.position += (forward * cameraComponent.direction + sideways * cameraComponent.right) * cameraComponent.speed * deltaTime;
				cameraComponent.target += (forward * cameraComponent.direction + sideways * cameraComponent.right) * cameraComponent.speed * deltaTime;

				if (inputManager->IsButtonHeld(GLFW_MOUSE_BUTTON_RIGHT))
				{
					auto deltaPos = inputManager->GetMouseDelta();

					cameraComponent.yaw += cameraComponent.sensitivity * inputManager->GetMouseDelta().first;
					cameraComponent.pitch += cameraComponent.sensitivity * -1 * inputManager->GetMouseDelta().second;
					cameraComponent.pitch = glm::clamp(cameraComponent.pitch, -89.f, 89.f);

					glm::vec3 direction{
						cosf(glm::radians(cameraComponent.yaw)) * cosf(glm::radians(cameraComponent.pitch)),
						sinf(glm::radians(cameraComponent.pitch)),
						sinf(glm::radians(cameraComponent.yaw)) * cosf(glm::radians(cameraComponent.pitch))
					};

					cameraComponent.target = cameraComponent.position + direction;
				}

				cameraComponent.direction = glm::normalize(cameraComponent.target - cameraComponent.position);
				cameraComponent.right = glm::normalize(glm::cross(cameraComponent.direction, cameraComponent.up));
				
				cameraComponent.view = glm::lookAt(cameraComponent.position, cameraComponent.target, cameraComponent.up);
				cameraComponent.viewInv = glm::inverse(cameraComponent.view);
				cameraComponent.proj = glm::perspective(glm::radians(cameraComponent.fov), cameraComponent.width / cameraComponent.height, cameraComponent.nearPlane, cameraComponent.farPlane);
				cameraComponent.projInv = glm::inverse(cameraComponent.proj);
				cameraComponent.viewProj = cameraComponent.proj * cameraComponent.view;
				cameraComponent.viewProjInv = glm::inverse(cameraComponent.viewProj);

				auto cameraGLSL = CameraGLSL(cameraComponent);
				//camDataSsboHandler[index] = cameraGLSL;

				auto cameraUbo = resourceManager->GetUbo("CameraData");
				cameraUbo->BufferSubStorage(0, sizeof(CameraGLSL), &cameraGLSL);

				cameraPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);

}

void CameraSystem::OnEnd(std::shared_ptr<Registry> registry)
{
}

nlohmann::json CameraSystem::Serialize(Registry* registry, Entity entity)
{
	nlohmann::json data;
	return data;
}

void CameraSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
}