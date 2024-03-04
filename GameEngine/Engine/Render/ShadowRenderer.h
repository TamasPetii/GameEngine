#pragma once
#include <memory>
#include <algorithm>
#include <execution>
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/Components.h"
#include "Render/OpenGL/Camera.h"

class ShadowRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry);
private:
	static void RenderDirLightShadows(std::shared_ptr<Registry> registry);
	static void RenderPointLightShadows(std::shared_ptr<Registry> registry);
	static void RenderSpotLightShadows(std::shared_ptr<Registry> registry);
};

