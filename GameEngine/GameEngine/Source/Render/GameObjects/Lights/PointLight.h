#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(const glm::vec3& position);

	//Getter
	inline glm::vec3& GetPositionRef() { return mPosition; }
private:
	glm::vec3 mPosition;
};

