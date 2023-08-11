#pragma once
#include "Light.h"

class Point {};

template<>
class Light<Point> : LightBase
{
public:
	Light();
	Light(const glm::vec3& position);
private:
	glm::vec3 mPosition;
};