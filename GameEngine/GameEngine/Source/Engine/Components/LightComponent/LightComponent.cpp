#include "LightComponent.h"

LightComponent::LightComponent()
{
	m_LightSource = nullptr;
}

LightComponent::~LightComponent()
{
	if(m_LightSource)
		delete m_LightSource;
}