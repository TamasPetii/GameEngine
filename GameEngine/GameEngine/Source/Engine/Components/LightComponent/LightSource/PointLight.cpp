#include "PointLight.h"

PointLight::PointLight() : PointLight(glm::vec3(1,1,1))
{
}

PointLight::PointLight(const glm::vec3& position)
{
	m_Position = position;
}

PointLight::PointLight(const json& object)
{
	LoadFromJson(object);
}

PointLight* PointLight::Clone() const
{
	return new PointLight(*this);
}

json PointLight::SaveToJson() const
{
	json data;
	data["PointLight"]["Position"] = Vec3_ToJson(m_Position);
	data["PointLight"]["Color"] = Vec3_ToJson(m_Color);
	data["PointLight"]["DiffuseIntensity"] = m_DiffuseIntensity;
	data["PointLight"]["SpecularIntensity"] = m_SpecularIntensity;
	data["PointLight"]["CastShadow"] = m_CastShadow;
	return data;
}

void PointLight::LoadFromJson(const json& object)
{
	m_Position = Vec3_FromJson(object["Position"]);
	m_Color = Vec3_FromJson(object["Color"]);
	m_DiffuseIntensity = object["DiffuseIntensity"];
	m_SpecularIntensity = object["SpecularIntensity"];
	m_CastShadow = object["CastShadow"];
}