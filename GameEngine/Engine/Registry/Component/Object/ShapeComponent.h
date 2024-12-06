#pragma once
#include "EngineApi.h"
#include <memory>
#include <glm/glm.hpp>
#include <Render/Geometry/Geometry.h>
#include <PhysX/PxPhysicsAPI.h>

class ENGINE_API ShapeComponent
{
public:
	bool toRender{ true };
	bool castShadow{ true };
	bool isInstanced{ false };
	bool receiveShadow{ true };
	bool useFrustumCulling{ true };
	std::shared_ptr<Geometry> shape = nullptr;
};

struct ENGINE_API ShapeGLSL
{
	ShapeGLSL(const ShapeComponent& component)
	{
		this->data.x = component.castShadow;
		this->data.y = component.receiveShadow;
	}

	glm::vec4 data;
};