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
	glm::vec3 translate{0.f, 0.f, 0.f};
	glm::vec3 rotate{0.f, 0.f, 0.f};
	glm::vec3 scale{1.f, 1.f, 1.f};
};

class TransformGLSL
{
public:
	TransformGLSL(const TransformComponent& component) : model(component.modelTransform), modelIT(component.modelTransformIT) {}
	glm::mat4 model;
	glm::mat4 modelIT;
};