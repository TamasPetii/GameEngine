#pragma once
#include <glm/glm.hpp>

class Collider
{
public:
	virtual ~Collider() = default;
	virtual glm::vec3 FindFurthestPoint(const glm::vec3& direction) const = 0;
};