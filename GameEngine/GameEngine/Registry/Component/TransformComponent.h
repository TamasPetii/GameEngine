#pragma once
#include <glm/glm.hpp>
#include <Registry/Component/Component.h>

class TransformComponent : public Component
{
public:
	glm::mat4 fullTransform;
	glm::mat4 fullTransformIT;
	glm::mat4 modelTransform;
	glm::mat4 modelTransformIT;
	glm::vec3 translate;
	glm::vec3 rotate;
	glm::vec3 scale;
};

class TransformGLSL
{
public:
	TransformGLSL(const TransformComponent& component) : model(component.modelTransform), modelIT(component.modelTransformIT) {}
	glm::mat4 model;
	glm::mat4 modelIT;
};