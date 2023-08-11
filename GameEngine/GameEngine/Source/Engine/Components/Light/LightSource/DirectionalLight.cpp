#include "DirectionalLight.h"

Light<Directional>::Light() : Light(glm::vec3(- 1, -1, -1))
{
}

Light<Directional>::Light(const glm::vec3& direction)
{
	mDirection = direction;
	mColor = glm::vec3(1,1,1);
	mDiffuseIntensity = 1;
	mSpecularIntensity = 1;
}