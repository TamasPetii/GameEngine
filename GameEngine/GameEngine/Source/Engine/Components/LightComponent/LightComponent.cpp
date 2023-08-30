#include "LightComponent.h"


LightComponent::LightComponent()
{
	m_LightSource = nullptr;
}

LightComponent::LightComponent(const json& object)
{
	LoadFromJson(object);
}

LightComponent::~LightComponent()
{
	delete m_LightSource;
}

json LightComponent::SaveToJson() const
{
	json data;
	data["LightComponent"] = m_LightSource->SaveToJson();
	return data;
}

void LightComponent::LoadFromJson(const json& object)
{
	if (object.find("DirectionLight") != object.end())
		m_LightSource = new DirectionLight(object["DirectionLight"]);
	else if (object.find("PointLight") != object.end())
		m_LightSource = new PointLight(object["PointLight"]);
}