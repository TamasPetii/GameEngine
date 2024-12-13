#pragma once
#include "EngineApi.h"
#include <glm/glm.hpp>

struct ENGINE_API TransformComponent
{
	TransformComponent();

	glm::mat4 fullTransform;
	glm::mat4 fullTransformIT;
	glm::mat4 modelTransform;
	glm::mat4 modelTransformIT;
	glm::mat4 translateMatrix;
	glm::mat4 rotateMatrix;
	glm::mat4 scaleMatrix;
	glm::vec3 translate;
	glm::vec3 rotate;
	glm::vec3 scale;
};

struct ENGINE_API TransformGLSL
{
	TransformGLSL(const TransformComponent& component);
	glm::mat4 model;
	glm::mat4 modelIT;
};