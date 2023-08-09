#pragma once
#include "Light.h"

class DirectionLight : public Light
{
public:
	DirectionLight();
	DirectionLight(const glm::vec3& direction);
	void Render() override {}

	//Getter
	inline glm::vec3& GetDirectionRef() { return mDirection; }
private:
	glm::vec3 mDirection;
};

