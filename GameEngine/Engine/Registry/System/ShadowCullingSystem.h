#pragma once
#include "EngineApi.h"
#include "Registry/System/System.h"

class Registry;

class ENGINE_API ShadowCullingSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
private:
	static void PointLightShadowCulling(std::shared_ptr<Registry> registry);
	static void SpotLightShadowCulling(std::shared_ptr<Registry> registry);
};

