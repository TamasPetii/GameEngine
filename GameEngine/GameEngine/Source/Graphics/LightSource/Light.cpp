#include "Light.h"

Light::Light() :
	m_Color(glm::vec3(1, 1, 1)),
	m_CastShadow(false),
	m_DiffuseIntensity(1.f),
	m_SpecularIntensity(1.f)
{
}