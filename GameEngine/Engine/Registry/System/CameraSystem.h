#pragma once
#include "EngineApi.h"
#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Manager/ResourceManager.h"
#include "Manager/InputManager.h"

#include "System.h"
#include "Registry/Registry.h"
#include "Registry/Component/CameraComponent.h"

class ENGINE_API CameraSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry, float deltaTime);
	static void OnEnd(std::shared_ptr<Registry> registry);

	static void UpdateToGpu(CameraComponent& cameraComponent);
	static void InvertPitch(CameraComponent& cameraComponent);
	static void UpdateMatrices(CameraComponent& cameraComponent);
	static Index GetMainCameraIndex(std::shared_ptr<Registry> registry);
	static CameraComponent& GetMainCamera(std::shared_ptr<Registry> registry);

	static nlohmann::json Serialize(Registry* registry, Entity entity);
	static void DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data);
};

