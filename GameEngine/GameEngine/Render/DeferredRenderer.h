#pragma once
#include <memory>
<<<<<<< HEAD
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
=======
#include <Registry/Registry.h>
#include <Manager/ResourceManager.h>
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

class DeferredRenderer
{
public:
<<<<<<< HEAD
	static void Render(std::shared_ptr<Registry> registry);
private:
	static void RenderDirectionLights(std::shared_ptr<Registry> registry);
	static void RenderPointLightsStencil(std::shared_ptr<Registry> registry);
	static void RenderPointLights(std::shared_ptr<Registry> registry);
	static void RenderSpotLightsStencil(std::shared_ptr<Registry> registry);
	static void RenderSpotLights(std::shared_ptr<Registry> registry);
=======
	static void Render(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager);
private:
	static void RenderDirectionLights(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager);
	static void RenderPointLightsStencil(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager);
	static void RenderPointLights(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager);
	static void RenderSpotLightsStencil(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager);
	static void RenderSpotLights(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager);
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
};

