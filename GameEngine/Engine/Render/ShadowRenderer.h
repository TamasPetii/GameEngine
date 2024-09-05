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
	static void RenderDirLightShadowModelInstanced(std::shared_ptr<Registry> registry);

	static void RenderPointLightShadowShapes(std::shared_ptr<Registry> registry, const PointLightComponent& pointLight);
	static void RenderPointLightShadowShapesInstanced(std::shared_ptr<Registry> registry, const PointLightComponent& pointLight);
	static void RenderPointLightShadowModel(std::shared_ptr<Registry> registry, const PointLightComponent& pointLight);
	static void RenderPointLightShadowModelInstanced(std::shared_ptr<Registry> registry, const PointLightComponent& pointLight);

	static void RenderSpotLightShadowShapes(std::shared_ptr<Registry> registry, const SpotLightComponent& spotLight);
	static void RenderSpotLightShadowShapesInstanced(std::shared_ptr<Registry> registry, const SpotLightComponent& spotLight);
	static void RenderSpotLightShadowModel(std::shared_ptr<Registry> registry, const SpotLightComponent& spotLight);
	static void RenderSpotLightShadowModelInstanced(std::shared_ptr<Registry> registry, const SpotLightComponent& spotLight);
};

