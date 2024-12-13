#pragma once
#include "EngineApi.h"
#include <memory>
#include <glm/glm.hpp>

class Model;

struct ENGINE_API ModelComponent
{
	ModelComponent();

	int lodLevel;
	bool automaticLod;
	bool toRender;
	bool castShadow;
	bool isInstanced;
	bool receiveShadow;
	bool useFrustumCulling;
	std::shared_ptr<Model> model;
};

struct ENGINE_API ModelGLSL
{
	ModelGLSL(const ModelComponent& component);

	bool receiveShadow;
};