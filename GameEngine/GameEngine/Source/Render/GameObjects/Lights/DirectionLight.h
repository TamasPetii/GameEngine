#pragma once
#include "Light.h"

class DirectionLight : public Light
{
public:
	DirectionLight();
	DirectionLight(const glm::vec3& direction);
	void Render() override {}

	//Getter
	glm::mat4 GetViewProjMatrix();
	inline glm::vec3& GetDirectionRef() { return mDirection; }
	glm::mat4 mProjMatrix;
	glm::mat4 mViewMatrix;
	glm::vec3 mPosition;
	glm::vec3 mDirection;
	glm::vec2 mProjX;
	glm::vec2 mProjY;
	glm::vec2 mProjZ;
private:
};

