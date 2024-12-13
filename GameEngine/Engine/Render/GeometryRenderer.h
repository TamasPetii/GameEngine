#pragma once
#include "EngineApi.h"
#include <memory>

class Registry;

class ENGINE_API GeometryRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry);
private:
	static void RenderShapes(std::shared_ptr<Registry> registry);
	static void RenderShapesInstanced(std::shared_ptr<Registry> registry);
	static void RenderModel(std::shared_ptr<Registry> registry);
	static void RenderModelInstanced(std::shared_ptr<Registry> registry);
	static void RenderModelAnimated(std::shared_ptr<Registry> registry);
};

