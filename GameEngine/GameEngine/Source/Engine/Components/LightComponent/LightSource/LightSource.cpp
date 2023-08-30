#include "LightSource.h"

LightSource::LightSource()
{
	m_Color = glm::vec3(1, 1, 1);
	m_CastShadow = false;
	m_DiffuseIntensity = 1;
	m_SpecularIntensity = 1;
}