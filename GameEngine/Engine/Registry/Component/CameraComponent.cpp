#include "CameraComponent.h"

CameraComponent::CameraComponent()
{
	isMain = false;
	enableUserMovement = true;

	speed = 25.f;
	sensitivity = 0.5f;

	width = 1920;
	height = 1080;
	fov = 60.f;
	nearPlane = 0.01f;
	farPlane = 500.f;

	position = glm::vec3(0.f, 0.f, 0.f);
	target = glm::vec3(0.f, 0.f, -1.f);
	up = glm::vec3(0.f, 1.f, 0.f);

	direction = glm::normalize(target - position);
	right = glm::normalize(glm::cross(direction, up));
	yaw = glm::degrees(atan2f(direction.z, direction.x));
	pitch = glm::degrees(asinf(direction.y));

	view = glm::mat4(1.f);
	proj = glm::mat4(1.f);
	viewInv = glm::mat4(1.f);
	projInv = glm::mat4(1.f);
	viewProj = glm::mat4(1.f);
	viewProjInv = glm::mat4(1.f);
}