#include "PointLight.h"

PointLight::PointLight()
{
	mPosition = glm::vec3(5, 5, 5);
}

PointLight::PointLight(const glm::vec3& position)
{
	mPosition = position;
}