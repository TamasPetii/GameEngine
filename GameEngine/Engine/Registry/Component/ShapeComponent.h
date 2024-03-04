#pragma once
#include "EngineApi.h"
#include <memory>
#include <glm/glm.hpp>
#include <Registry/Component/Component.h>
#include <Geometry/Geometry.h>

class ENGINE_API ShapeComponent : public Component
{
public:
	bool toRender{ true };
	bool castShadow{ true };
	bool isInstanced{ false };
	bool receiveShadow{ true };
	bool useFrustumCulling{ true };
	std::shared_ptr<Geometry> shape;
};