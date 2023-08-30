#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
	m_Translation = glm::vec3(0);
	m_Rotation = glm::vec3(0);
	m_Scale = glm::vec3(1);
}

TransformComponent::TransformComponent(const json& object)
{
	LoadFromJson(object);
}

const glm::mat4 TransformComponent::Get_TransformMatrix() const
{
	return glm::translate(m_Translation)
		* glm::rotate(glm::radians(m_Rotation.x), glm::vec3(1, 0, 0))
		* glm::rotate(glm::radians(m_Rotation.y), glm::vec3(0, 1, 0))
		* glm::rotate(glm::radians(m_Rotation.z), glm::vec3(0, 0, 1))
		* glm::scale(m_Scale);
}

json TransformComponent::SaveToJson() const
{
	json data;
	data["TransformComponent"]["Translation"] = Vec3_ToJson(m_Translation);
	data["TransformComponent"]["Rotation"] = Vec3_ToJson(m_Rotation);
	data["TransformComponent"]["Scale"] = Vec3_ToJson(m_Scale);
	return data;
}

void TransformComponent::LoadFromJson(const json& object)
{
	m_Translation = Vec3_FromJson(object["Translation"]);
	m_Rotation = Vec3_FromJson(object["Rotation"]);
	m_Scale = Vec3_FromJson(object["Scale"]);
}