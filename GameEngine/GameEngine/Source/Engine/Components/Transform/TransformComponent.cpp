#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
	mTranslation = glm::vec3(0);
	mRotation = glm::vec3(0);
	mScale = glm::vec3(1);
}

glm::mat4 TransformComponent::GetTransformMatrix()
{
	return glm::translate(mTranslation)
		 * glm::rotate(glm::radians(mRotation.x), glm::vec3(1, 0, 0))
		 * glm::rotate(glm::radians(mRotation.y), glm::vec3(0, 1, 0))
		 * glm::rotate(glm::radians(mRotation.z), glm::vec3(0, 0, 1))
		 * glm::scale(mScale);
}
