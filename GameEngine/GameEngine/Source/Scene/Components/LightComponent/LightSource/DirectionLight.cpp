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

DirectionLight::DirectionLight(const json& object)
{
	LoadFromJson(object);
}

DirectionLight* DirectionLight::Clone() const
{
	return new DirectionLight(*this);
}

json DirectionLight::SaveToJson() const
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

void DirectionLight::LoadFromJson(const json& object)
{
	m_Direction = Vec3_FromJson(object["Direction"]);
	m_Color = Vec3_FromJson(object["Color"]);
	m_DiffuseIntensity = object["DiffuseIntensity"];
	m_SpecularIntensity = object["SpecularIntensity"];
	m_CastShadow = object["CastShadow"];
	m_ShadowBoxX = Vec2_FromJson(object["ShadowBoxX"]);
	m_ShadowBoxY = Vec2_FromJson(object["ShadowBoxY"]);
	m_ShadowBoxZ = Vec2_FromJson(object["ShadowBoxZ"]);
}