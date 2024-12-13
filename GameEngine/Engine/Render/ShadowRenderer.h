#pragma once
#include "EngineApi.h"
#include "Registry/Entity.h"
#include <memory>
#include <vector>

class Registry;

enum class ShadowType
{
	DIRECTION,
	POINT,
	SPOT
};

class ENGINE_API ShadowRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry);
private:
	static void RenderDirLightShadows(std::shared_ptr<Registry> registry);
	static void RenderPointLightShadows(std::shared_ptr<Registry> registry);
	static void RenderSpotLightShadows(std::shared_ptr<Registry> registry);

	static void RenderShapesShadow(ShadowType type, std::shared_ptr<Registry> registry, std::vector<char>& visibleEntities);
	static void RenderShapesInstancedShadow(ShadowType type, std::shared_ptr<Registry> registry, std::vector<char>& visibleEntities);
	static void RenderModelShadow(ShadowType type, std::shared_ptr<Registry> registry, std::vector<char>& visibleEntities);
	static void RenderModelInstancedShadow(ShadowType type, std::shared_ptr<Registry> registry, std::vector<char>& visibleEntities);
	static void RenderAnimationShadow(ShadowType type, std::shared_ptr<Registry> registry, std::vector<char>& visibleEntities);

	static void RenderShapesShadow(ShadowType type, std::shared_ptr<Registry> registry, std::vector<Entity>& visibleEntities);
	static void RenderShapesInstancedShadow(ShadowType type, std::shared_ptr<Registry> registry, std::vector<Entity>& visibleEntities);
	static void RenderModelShadow(ShadowType type, std::shared_ptr<Registry> registry, std::vector<Entity>& visibleEntities);
	static void RenderModelInstancedShadow(ShadowType type, std::shared_ptr<Registry> registry, std::vector<Entity>& visibleEntities);
};

