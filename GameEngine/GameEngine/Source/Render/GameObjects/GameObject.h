#pragma once
#define _USE_MATH_DEFINES
#include "../../Engine/Engine.h"
#include <cmath>

class GameObject
{
public:
	GameObject();
	virtual void Render() = 0;
	//Getter
	inline int GetId() { return mId; }
	inline glm::mat4 GetTransformMatrix() { return glm::translate(mTranslate) * ((mRotation.x == 0 && mRotation.y == 0 && mRotation.z == 0) ? glm::mat4(1) : glm::rotate(glm::radians(mRotationAngle), mRotation)) * glm::scale(mScale); }
	inline glm::vec3& GetTranslateRef() { return mTranslate; }
	inline glm::vec3& GetRotationRef() { return mRotation; }
	inline glm::vec3& GetScaleRef() { return mScale; }
	inline float& GetAngleRef() { return mRotationAngle; }
	inline std::string& GetNameRef() { return mName; }
protected:
	static int GenerateUniqueId();
	int mId;
	float mRotationAngle;
	glm::vec3 mTranslate;
	glm::vec3 mRotation;
	glm::vec3 mScale;
	std::string mName;
};

