#pragma once
#include "EngineApi.h"

#include <mutex>
#include <memory>
#include <algorithm>
#include <execution>

#include "Registry/Registry.h"
#include "Manager/ModelManager.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/Object/ShapeComponent.h"
#include "Registry/Component/Object/ModelComponent.h"
#include "Registry/Component/TransformComponent.h"
#include "Registry/Component/Object/AnimationComponent.h"

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

