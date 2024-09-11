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
				
				if (cameraComponent.enableUserMovement)
				{
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
				}

				cameraComponent.direction = glm::normalize(cameraComponent.target - cameraComponent.position);
				cameraComponent.right = glm::normalize(glm::cross(cameraComponent.direction, cameraComponent.up));
				
				cameraComponent.view = glm::lookAt(cameraComponent.position, cameraComponent.target, cameraComponent.up);
				cameraComponent.viewInv = glm::inverse(cameraComponent.view);
				cameraComponent.proj = glm::perspective(glm::radians(cameraComponent.fov), cameraComponent.width / cameraComponent.height, cameraComponent.nearPlane, cameraComponent.farPlane);
				cameraComponent.projInv = glm::inverse(cameraComponent.proj);
				cameraComponent.viewProj = cameraComponent.proj * cameraComponent.view;
				cameraComponent.viewProjInv = glm::inverse(cameraComponent.viewProj);

				if (cameraComponent.isMain)
				{
					auto cameraUbo = resourceManager->GetUbo("CameraData");
					auto cameraGLSL = CameraGLSL(cameraComponent);
					cameraUbo->BufferSubStorage(0, sizeof(CameraGLSL), &cameraGLSL);
				}

				cameraPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);

}

void CameraSystem::OnEnd(std::shared_ptr<Registry> registry)
{
}

CameraComponent& CameraSystem::GetMainCamera(std::shared_ptr<Registry> registry)
{
	static CameraComponent basicCamera;

	auto cameraPool = registry->GetComponentPool<CameraComponent>();

	if (!cameraPool)
		return basicCamera;

	auto it = std::find_if(cameraPool->GetDenseComponentsArray().begin(), cameraPool->GetDenseComponentsArray().end(),
		[&](const CameraComponent& component) -> bool {
			return component.isMain;
		});

	if (it != cameraPool->GetDenseComponentsArray().end())
		return *it;
	
	return basicCamera;
}

nlohmann::json CameraSystem::Serialize(Registry* registry, Entity entity)
{
	auto& cameraComponent = registry->GetComponent<CameraComponent>(entity);

	nlohmann::json data;
	data["isMain"] = cameraComponent.isMain;
	data["enableUserMovement"] = cameraComponent.enableUserMovement;
	data["nearPlane"] = cameraComponent.nearPlane;
	data["farPlane"] = cameraComponent.farPlane;
	data["yaw"] = cameraComponent.yaw;
	data["pitch"] = cameraComponent.pitch;
	data["fov"] = cameraComponent.fov;
	data["width"] = cameraComponent.width;
	data["height"] = cameraComponent.height;
	data["speed"] = cameraComponent.speed;
	data["sensitivity"] = cameraComponent.sensitivity;

	data["up"]["x"] = cameraComponent.up.x;
	data["up"]["y"] = cameraComponent.up.y;
	data["up"]["z"] = cameraComponent.up.z;

	data["target"]["x"] = cameraComponent.target.x;
	data["target"]["y"] = cameraComponent.target.y;
	data["target"]["z"] = cameraComponent.target.z;

	data["position"]["x"] = cameraComponent.position.x;
	data["position"]["y"] = cameraComponent.position.y;
	data["position"]["z"] = cameraComponent.position.z;

	return data;
}

void CameraSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& cameraComponent = registry->GetComponent<CameraComponent>(entity);

	cameraComponent.isMain = data["isMain"];
	cameraComponent.enableUserMovement = data["enableUserMovement"];
	cameraComponent.nearPlane = data["nearPlane"];
	cameraComponent.farPlane = data["farPlane"];
	cameraComponent.yaw = data["yaw"];
	cameraComponent.pitch = data["pitch"];
	cameraComponent.fov = data["fov"];
	cameraComponent.width = data["width"];
	cameraComponent.height = data["height"];
	cameraComponent.speed = data["speed"];
	cameraComponent.sensitivity = data["sensitivity"];

	cameraComponent.up.x = data["up"]["x"];
	cameraComponent.up.y = data["up"]["y"];
	cameraComponent.up.z = data["up"]["z"];

	cameraComponent.target.x = data["target"]["x"];
	cameraComponent.target.y = data["target"]["y"];
	cameraComponent.target.z = data["target"]["z"];

	cameraComponent.position.x = data["position"]["x"];
	cameraComponent.position.y = data["position"]["y"];
	cameraComponent.position.z = data["position"]["z"];

	registry->SetFlag<CameraComponent>(entity, UPDATE_FLAG);
}