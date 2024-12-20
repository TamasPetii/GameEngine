#include "CameraSystem.h"

#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Registry/Registry.h"
#include "Manager/InputManager.h"
#include "Manager/ResourceManager.h"
#include "Settings/GlobalSettings.h"
#include "Registry/Component/CameraComponent.h"
#include "Render/OpenGL/ShaderStorageBufferGL.h"
#include "Settings/GlobalSettings.h"
#include "Render/WireframeRenderer.h"

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

	auto camDataWireframeSsbo = resourceManager->GetSsbo("CameraWireframeData");
	if (camDataWireframeSsbo->GetBufferHandler() == nullptr)
		camDataWireframeSsbo->MapBufferRange();
	glm::mat4* camDataWireframeSsboHandler = static_cast<glm::mat4*>(camDataWireframeSsbo->GetBufferHandler());

	auto camDataBillboardSsbo = resourceManager->GetSsbo("CameraBillboardData");
	if (camDataBillboardSsbo->GetBufferHandler() == nullptr)
		camDataBillboardSsbo->MapBufferRange();
	glm::vec4* camDataBillboardSsboHandler = static_cast<glm::vec4*>(camDataBillboardSsbo->GetBufferHandler());

	if (!camDataWireframeSsboHandler || !camDataBillboardSsboHandler || cameraPool->GetSize() > resourceManager->GetComponentSsboSize<CameraComponent>())
		return;

	std::for_each(std::execution::seq, cameraPool->GetDenseEntitiesArray().begin(), cameraPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (true || cameraPool->IsFlagSet(entity, UPDATE_FLAG))
			{
				auto& cameraComponent = cameraPool->GetComponent(entity);
				auto index = cameraPool->GetIndex(entity);

				float forward = 0;
				float sideways = 0;
				float distance = 0;

				if (cameraComponent.enableUserMovement)
				{
					if (GlobalSettings::IsViewPortActive && cameraComponent.isMain)
					{
						if (inputManager->IsKeyHeld(KEY_W))
							forward = 1;
						if (inputManager->IsKeyHeld(KEY_S))
							forward = -1;
						if (inputManager->IsKeyHeld(KEY_D))
							sideways = 1;
						if (inputManager->IsKeyHeld(KEY_A))
							sideways = -1;
						if (inputManager->IsButtonHeld(BUTTON_RIGHT))
						{
							auto deltaPos = inputManager->GetMouseDelta();
							cameraComponent.yaw += cameraComponent.sensitivity * deltaPos.first;
							cameraComponent.pitch += cameraComponent.sensitivity * -1 * deltaPos.second;
							cameraComponent.pitch = glm::clamp(cameraComponent.pitch, -89.f, 89.f);
						}
					}

					glm::vec3 direction{
						glm::cos(glm::radians(cameraComponent.yaw)) * glm::cos(glm::radians(cameraComponent.pitch)),
						glm::sin(glm::radians(cameraComponent.pitch)),
						glm::sin(glm::radians(cameraComponent.yaw)) * glm::cos(glm::radians(cameraComponent.pitch))
					};

					cameraComponent.direction = glm::normalize(direction);
				}

				if (cameraComponent.isOrbital)
				{
					cameraComponent.position = cameraComponent.target - cameraComponent.direction * cameraComponent.distance;
					cameraComponent.right = glm::normalize(glm::cross(cameraComponent.direction, cameraComponent.up));
				}
				else
				{
					cameraComponent.right = glm::normalize(glm::cross(cameraComponent.direction, cameraComponent.up));
					cameraComponent.position += (forward * cameraComponent.direction + sideways * cameraComponent.right) * cameraComponent.speed * deltaTime;
					cameraComponent.target = cameraComponent.position + cameraComponent.direction;
				}

				
				UpdateMatrices(cameraComponent);

				if (cameraComponent.isMain)
				{
					UpdateToGpu(cameraComponent);
				}

				if (!GlobalSettings::GameViewActive)
				{
					camDataBillboardSsboHandler[index] = glm::vec4(cameraComponent.position.x, cameraComponent.position.y, cameraComponent.position.z, entity);
				}

				if (!GlobalSettings::GameViewActive && WireframeRenderer::ShowCameraVolume)
				{
					camDataWireframeSsboHandler[index] = cameraComponent.viewProjInv;
				}

				cameraPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);
}

void CameraSystem::OnEnd(std::shared_ptr<Registry> registry)
{
}

Index CameraSystem::GetMainCameraIndex(std::shared_ptr<Registry> registry)
{
	auto cameraPool = registry->GetComponentPool<CameraComponent>();

	if (!cameraPool)
		return null;

	auto it = std::find_if(cameraPool->GetDenseEntitiesArray().begin(), cameraPool->GetDenseEntitiesArray().end(),
		[&](const Entity entity) -> bool {
			return cameraPool->GetComponent(entity).isMain;
		});

	if (it != cameraPool->GetDenseEntitiesArray().end())
	{
		Entity entity = *it;
		return cameraPool->GetIndex(entity);
	}

	return null;
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

void CameraSystem::UpdateMatrices(CameraComponent& cameraComponent)
{
	cameraComponent.view = glm::lookAt(cameraComponent.position, cameraComponent.target, cameraComponent.up);
	cameraComponent.viewInv = glm::inverse(cameraComponent.view);
	cameraComponent.proj = glm::perspective(glm::radians(cameraComponent.fov), cameraComponent.width / cameraComponent.height, cameraComponent.nearPlane, cameraComponent.farPlane);
	cameraComponent.projInv = glm::inverse(cameraComponent.proj);
	cameraComponent.viewProj = cameraComponent.proj * cameraComponent.view;
	cameraComponent.viewProjInv = glm::inverse(cameraComponent.viewProj);
}

void CameraSystem::UpdateToGpu(CameraComponent& cameraComponent)
{
	auto resourceManager = ResourceManager::Instance();

	auto camDataSsbo = resourceManager->GetSsbo("CameraData");
	if (camDataSsbo->GetBufferHandler() == nullptr)
		camDataSsbo->MapBufferRange();
	CameraGLSL* camDataSsboHandler = static_cast<CameraGLSL*>(camDataSsbo->GetBufferHandler());

	if (!camDataSsboHandler)
		return;

	auto cameraGLSL = CameraGLSL(cameraComponent);
	std::memcpy(camDataSsboHandler, &cameraGLSL, sizeof(CameraGLSL));

	camDataSsbo->UnMapBuffer();
}

void CameraSystem::InvertPitch(CameraComponent& cameraComponent)
{
	cameraComponent.pitch = -cameraComponent.pitch;

	glm::vec3 direction{
		cosf(glm::radians(cameraComponent.yaw)) * cosf(glm::radians(cameraComponent.pitch)),
		sinf(glm::radians(cameraComponent.pitch)),
		sinf(glm::radians(cameraComponent.yaw)) * cosf(glm::radians(cameraComponent.pitch))
	};

	cameraComponent.target = cameraComponent.position + direction;
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