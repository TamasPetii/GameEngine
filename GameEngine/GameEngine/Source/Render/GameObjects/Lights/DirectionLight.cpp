#include "DirectionLight.h"

DirectionLight::DirectionLight()
{
	mName = "DirectionLight";
	mDirection = glm::vec3(-1, -1, -1);
}

DirectionLight::DirectionLight(const glm::vec3& direction)
{
	mName = "DirectionLight";
	mDirection = direction;
}