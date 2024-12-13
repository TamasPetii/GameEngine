#pragma once
#include "EngineApi.h"
#include "Registry/System/System.h"

class Registry;
struct DefaultCollider;

class ENGINE_API FrustumCullingSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
private:
	static void GeometryCulling(std::shared_ptr<Registry> registry, DefaultCollider& cameraFrustum);
	static void PointLightCulling(std::shared_ptr<Registry> registry, DefaultCollider& cameraFrustum);
	static void SpotLightCulling(std::shared_ptr<Registry> registry, DefaultCollider& cameraFrustum);
};

