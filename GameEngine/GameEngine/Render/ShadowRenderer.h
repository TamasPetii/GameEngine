#pragma once
#include <memory>
#include "Registry/Registry.h"

class ShadowRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry);
private:
	static void RenderDirLightShadows(std::shared_ptr<Registry> registry);
	static void RenderPointLightShadows(std::shared_ptr<Registry> registry);
	static void RenderSpotLightShadows(std::shared_ptr<Registry> registry);
};

