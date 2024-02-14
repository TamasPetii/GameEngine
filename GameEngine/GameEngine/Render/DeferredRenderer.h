#pragma once
#include <memory>
#include <Registry/Registry.h>
#include <Manager/ResourceManager.h>

class DeferredRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager);
private:
	static void RenderDirectionLights(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager);
	static void RenderPointLightsStencil(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager);
	static void RenderPointLights(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager);
	static void RenderSpotLightsStencil(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager);
	static void RenderSpotLights(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager);
};

