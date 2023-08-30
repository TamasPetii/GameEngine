#include "PointLight.h"

PointLight::PointLight() : PointLight(glm::vec3(1,1,1))
{
}

PointLight::PointLight(const glm::vec3& position)
{
	m_Position = position;
}