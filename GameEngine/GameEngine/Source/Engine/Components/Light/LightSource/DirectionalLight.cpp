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
	mShadowBoxX = glm::vec2(-10, 10);
	mShadowBoxY = glm::vec2(-10, 10);
	mShadowBoxZ = glm::vec2(-10, 10);
}