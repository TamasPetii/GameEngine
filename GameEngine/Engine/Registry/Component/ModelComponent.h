#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <Registry/Component/Component.h>
#include <Model/Model.h>

class ModelComponent : public Component
{
public:
	bool toRender{ true };
	bool castShadow{ true };
	bool isInstanced{ false };
	bool receiveShadow{ true };
	std::shared_ptr<Model> model;
};