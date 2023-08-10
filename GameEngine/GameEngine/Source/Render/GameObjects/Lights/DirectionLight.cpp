#include "DirectionLight.h"

DirectionLight::DirectionLight() : DirectionLight(glm::vec3(-1, -1, -1))
{

}

DirectionLight::DirectionLight(const glm::vec3& direction)
{
	mName = "DirectionLight";
	mTranslate = glm::vec3(4, 10, 8);
	mDirection = direction;
	mPosition = glm::vec3(5,5,5);
	mProjX.x = -10;
	mProjX.y = 10;
	mProjY.x = -10;
	mProjY.y = 10;
	mProjZ.x = -50;
	mProjZ.y = 100;
	mProjMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.f, 10.f);
	mViewMatrix = glm::lookAt(mPosition, mDirection, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 DirectionLight::GetViewProjMatrix()
{ 
	mViewMatrix = glm::lookAt(mPosition, mDirection, glm::vec3(0.0f, 1.0f, 0.0f));
	mProjMatrix = glm::ortho(mProjX.x, mProjX.y, mProjY.x, mProjY.y, mProjZ.x, mProjZ.y);
	return mProjMatrix * mViewMatrix;
}