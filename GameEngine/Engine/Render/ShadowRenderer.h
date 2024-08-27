#pragma once
#include <memory>
#include <algorithm>
#include <execution>

#include "EngineApi.h"
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Manager/ModelManager.h"
#include "Registry/Component/Components.h"
#include "Render/OpenGL/Camera.h"

class ENGINE_API ShadowRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry);
private:
	static void RenderDirLightShadows(std::shared_ptr<Registry> registry);
	static void RenderPointLightShadows(std::shared_ptr<Registry> registry);
	static void RenderSpotLightShadows(std::shared_ptr<Registry> registry);

	static void RenderDirLightShadowShapes(std::shared_ptr<Registry> registry);
	static void RenderDirLightShadowShapesInstanced(std::shared_ptr<Registry> registry);
	static void RenderDirLightShadowModel(std::shared_ptr<Registry> registry);
	static void RenderDirLightShadowModelInstance(std::shared_ptr<Registry> registry);
};

