#include "Light.h"

Light::Light() : GameObject()
{
	mColor = glm::vec3(1, 1, 1);
	mDiffuse = 1;
	mSpecular = 1;
	mVolume = 1;
}
