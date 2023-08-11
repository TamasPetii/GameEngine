#pragma once
#include "Light.h"

class Directional {};

template<>
class Light<Directional> : public LightBase
{
public:
	Light();
	Light(const glm::vec3& direction);
	glm::vec3& GetDirection() { return mDirection; }
private:
	glm::vec3 mDirection;
};

