#pragma once
#include "EngineApi.h"
#include <memory>
#include <glm/glm.hpp>
#include <Model/Model.h>

class ENGINE_API ModelComponent
{
public:
	int lodLevel = 0;
	bool toRender{ true };
	bool castShadow{ true };
	bool isInstanced{ false };
	bool receiveShadow{ true };
	bool useFrustumCulling{ true };
	std::shared_ptr<Model> model = nullptr;
};

struct ENGINE_API ModelGLSL
{
	ModelGLSL(const ModelComponent& component)
	{
		this->data.x = component.castShadow ? 1 : 0;
		this->data.y = component.receiveShadow ? 1 : 0;
	}

	glm::vec4 data;
};