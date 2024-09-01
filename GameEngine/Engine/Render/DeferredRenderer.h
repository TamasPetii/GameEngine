#pragma once
#include "EngineApi.h"

#include <memory>
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"

class ENGINE_API DeferredRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry);
private:
	static void RenderAmbientColors(std::shared_ptr<Registry> registry);
	static void RenderDirectionLights(std::shared_ptr<Registry> registry);
	static void RenderPointLightsStencil(std::shared_ptr<Registry> registry);
	static void RenderPointLights(std::shared_ptr<Registry> registry);
	static void RenderSpotLightsStencil(std::shared_ptr<Registry> registry);
	static void RenderSpotLights(std::shared_ptr<Registry> registry);
};

