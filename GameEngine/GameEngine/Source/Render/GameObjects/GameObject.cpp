#include "GameObject.h"

unsigned int GameObject::GenerateUniqueId()
{
	static unsigned int id = 0;
	return id++;
}

GameObject::GameObject()
{
	mId = GameObject::GenerateUniqueId();
	mTranslate = glm::vec3(0,0,0);
	mRotation = glm::vec3(0, 0, 0);
	mScale = glm::vec3(1, 1, 1);
	mRotationAngle = 90;
}