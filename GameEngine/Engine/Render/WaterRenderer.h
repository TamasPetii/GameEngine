#pragma once
#include "EngineApi.h"
#include <memory>

class Registry;

class ENGINE_API WaterRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry);
private:
	static void RenderWater(std::shared_ptr<Registry> registry);
	static void RenderPreWater(std::shared_ptr<Registry> registry);

	static void RenderShapes(std::shared_ptr<Registry> registry);
	static void RenderShapesInstanced(std::shared_ptr<Registry> registry);
	static void RenderModel(std::shared_ptr<Registry> registry);
	static void RenderModelInstanced(std::shared_ptr<Registry> registry);
	static void RenderAnimation(std::shared_ptr<Registry> registry);
	static void RenderSkybox(std::shared_ptr<Registry> registry);
};

