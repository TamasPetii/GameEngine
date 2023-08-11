#include "PointLight.h"

Light<Point>::Light() : Light(glm::vec3(2, 2, 2))
{
}

Light<Point>::Light(const glm::vec3& position)
{
	mPosition = position;
	mColor = glm::vec3(1, 1, 1);
	mDiffuseIntensity = 1;
	mSpecularIntensity = 1;
}