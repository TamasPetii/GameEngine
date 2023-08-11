#pragma once
#include "../Component.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/transform2.hpp>

class TransformComponent : public Component
{
public:
	TransformComponent();
	glm::mat4 GetTransformMatrix();
	inline glm::vec3& GetTranslation() { return mTranslation; }
	inline glm::vec3& GetRotation() { return mRotation; }
	inline glm::vec3& GetScale() { return mScale; }
private:
	glm::vec3 mTranslation;
	glm::vec3 mRotation;
	glm::vec3 mScale;
};

