#include "DirectionLight.h"

DirectionLight::DirectionLight() : DirectionLight(glm::vec3(-1, -1, -1))
{
}

DirectionLight::DirectionLight(const glm::vec3& direction)
{
	m_Direction = direction;
	m_ShadowBoxX = glm::vec2(-20, 20);
	m_ShadowBoxY = glm::vec2(-20, 20);
	m_ShadowBoxZ = glm::vec2(-50, 50);
}