#pragma once
#include <memory>
#include <algorithm>
#include <execution>
#include "Registry/Registry.h"
#include "Manager/ModelManager.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/ShapeComponent.h"
#include "Registry/Component/ModelComponent.h"
#include "Registry/Component/TransformComponent.h"

class GeometryRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry);
private:
	static void RenderShapes(std::shared_ptr<Registry> registry);
	static void RenderShapesInstanced(std::shared_ptr<Registry> registry);
	static void RenderModel(std::shared_ptr<Registry> registry);
	static void RenderModelInstanced(std::shared_ptr<Registry> registry);
};

