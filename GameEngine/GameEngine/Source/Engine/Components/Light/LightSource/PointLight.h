#pragma once
#include "Light.h"

class Point {};

template<>
class Light<Point> : public LightBase
{
public:
	Light();
	Light(const glm::vec3& position);
	inline glm::vec3& GetPosition() { return mPosition; }
private:
	glm::vec3 mPosition;
};