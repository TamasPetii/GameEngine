#pragma once
#include "EngineApi.h"
#include <glm/glm.hpp>

class ENGINE_API CameraComponent
{
public:
	CameraComponent();

	bool isMain;
	bool enableUserMovement;

	float nearPlane;
	float farPlane;
	float yaw;
	float pitch;
	float fov;
	float width;
	float height;
	float speed;
	float sensitivity;

	glm::vec3 up;
	glm::vec3 target;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 right;

	glm::mat4 view;
	glm::mat4 viewInv;
	glm::mat4 proj;
	glm::mat4 projInv;
	glm::mat4 viewProj;
	glm::mat4 viewProjInv;
};

class ENGINE_API CameraGLSL
{
public:
	CameraGLSL(const CameraComponent& component)
	{
		this->view = component.view;
		this->viewInv = component.viewInv;
		this->proj = component.proj;
		this->projInv = component.projInv;
		this->viewProj = component.viewProj;
		this->viewProjInv = component.viewProjInv;
		this->eye = glm::vec4(component.position.x, component.position.y, component.position.z, 1);
	}

	glm::mat4 view;
	glm::mat4 viewInv;
	glm::mat4 proj;
	glm::mat4 projInv;
	glm::mat4 viewProj;
	glm::mat4 viewProjInv;
	glm::vec4 eye;
};