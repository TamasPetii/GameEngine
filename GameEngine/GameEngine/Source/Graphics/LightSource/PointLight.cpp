#include "PointLight.h"

PointLight::PointLight() : PointLight(glm::vec3(1,1,1))
{
}

PointLight::PointLight(const glm::vec3& position)
{
	m_Position = position;
}

PointLight::PointLight(const json& data)
{
	DeSerialize(data);
}

PointLight* PointLight::Clone() const
{
	auto data = Serialize();
	return new PointLight(data["PointLight"]);
}

json PointLight::Serialize() const
{
	json data;
	data["PointLight"]["Position"] = Vec3_ToJson(m_Position);
	data["PointLight"]["Color"] = Vec3_ToJson(m_Color);
	data["PointLight"]["DiffuseIntensity"] = m_DiffuseIntensity;
	data["PointLight"]["SpecularIntensity"] = m_SpecularIntensity;
	data["PointLight"]["CastShadow"] = m_CastShadow;
	return data;
}

void PointLight::DeSerialize(const json& data)
{
	m_Position = Vec3_FromJson(data["Position"]);
	m_Color = Vec3_FromJson(data["Color"]);
	m_DiffuseIntensity = data["DiffuseIntensity"];
	m_SpecularIntensity = data["SpecularIntensity"];
	m_CastShadow = data["CastShadow"];
}