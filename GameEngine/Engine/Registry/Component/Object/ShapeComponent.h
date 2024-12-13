#pragma once
#include "EngineApi.h"
#include <memory>
#include <glm/glm.hpp>

class Geometry;

struct ENGINE_API ShapeComponent
{
	ShapeComponent();
	virtual ~ShapeComponent();

	int lodLevel;
	bool automaticLod;
	bool toRender;
	bool castShadow;
	bool isInstanced;
	bool receiveShadow;
	bool useFrustumCulling;
	std::shared_ptr<Geometry> shape;
};

struct ENGINE_API ShapeGLSL
{
	ShapeGLSL(const ShapeComponent& component);

	bool receiveShadow;
};