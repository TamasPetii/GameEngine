#include "GameObject.h"

int GameObject::GenerateUniqueId()
{
	static int id = 0;
	return id++;
}

GameObject::GameObject()
{
	mId = GameObject::GenerateUniqueId();
	mTranslate = glm::vec3(0,0,0);
	mRotation = glm::vec3(0, 0, 0);
	mScale = glm::vec3(1, 1, 1);
	mRotationAngle = 90;
	mName = "Empty";
}

glm::mat4 GameObject::GetTransformMatrix()
{
	return glm::translate(mTranslate)
		 * glm::rotate(glm::radians(mRotation.x), glm::vec3(1, 0, 0))
		 * glm::rotate(glm::radians(mRotation.y), glm::vec3(0, 1, 0))
		 * glm::rotate(glm::radians(mRotation.z), glm::vec3(0, 0, 1))
		 * glm::scale(mScale);
}