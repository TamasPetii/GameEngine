#include "DirectionLight.h"

DirectionLight::DirectionLight() : DirectionLight(glm::vec3(-0.8, -0.9, -1))
{
}

DirectionLight::DirectionLight(const glm::vec3& direction)
{
	m_Direction = direction;
	m_ShadowBoxX = glm::vec2(-40, 40);
	m_ShadowBoxY = glm::vec2(-40, 40);
	m_ShadowBoxZ = glm::vec2(-100, 100);
}

DirectionLight::DirectionLight(const json& data)
{
	DeSerialize(data);
}

DirectionLight* DirectionLight::Clone() const
{
	auto data = Serialize();
	return new DirectionLight(data["DirectionLight"]);
}

json DirectionLight::Serialize() const
{
	json data;
	data["DirectionLight"]["Direction"] = Vec3_ToJson(m_Direction);
	data["DirectionLight"]["Color"] = Vec3_ToJson(m_Color);
	data["DirectionLight"]["DiffuseIntensity"] = m_DiffuseIntensity;
	data["DirectionLight"]["SpecularIntensity"] = m_SpecularIntensity;
	data["DirectionLight"]["CastShadow"] = m_CastShadow;
	data["DirectionLight"]["ShadowBoxX"] = Vec2_ToJson(m_ShadowBoxX);
	data["DirectionLight"]["ShadowBoxY"] = Vec2_ToJson(m_ShadowBoxY);
	data["DirectionLight"]["ShadowBoxZ"] = Vec2_ToJson(m_ShadowBoxZ);
	return data;
}

void DirectionLight::DeSerialize(const json& data)
{
	m_Direction = Vec3_FromJson(data["Direction"]);
	m_Color = Vec3_FromJson(data["Color"]);
	m_DiffuseIntensity = data["DiffuseIntensity"];
	m_SpecularIntensity = data["SpecularIntensity"];
	m_CastShadow = data["CastShadow"];
	m_ShadowBoxX = Vec2_FromJson(data["ShadowBoxX"]);
	m_ShadowBoxY = Vec2_FromJson(data["ShadowBoxY"]);
	m_ShadowBoxZ = Vec2_FromJson(data["ShadowBoxZ"]);
}