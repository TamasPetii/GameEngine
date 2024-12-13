#pragma once
#include "EngineApi.h"
#include "Registry/System/System.h"
#include "Registry/Entity.h"

class Registry;
struct CameraComponent;

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

