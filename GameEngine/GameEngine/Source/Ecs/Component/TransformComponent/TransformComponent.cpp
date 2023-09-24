#include "TransformComponent.h"

namespace Ecs
{
	TransformComponent::TransformComponent() :
		m_Translation(glm::vec3(0)),
		m_Rotation(glm::vec3(0)),
		m_Scale(glm::vec3(1))
	{
	}

	TransformComponent::TransformComponent(const json& data)
	{
		DeSerialize(data);
	}

	json TransformComponent::Serialize() const
	{
		json data;
		data["TransformComponent"]["Translation"] = Vec3_ToJson(m_Translation);
		data["TransformComponent"]["Rotation"] = Vec3_ToJson(m_Rotation);
		data["TransformComponent"]["Scale"] = Vec3_ToJson(m_Scale);
		return data;
	}

	void TransformComponent::DeSerialize(const json& data)
	{
		m_Translation = Vec3_FromJson(data["Translation"]);
		m_Rotation = Vec3_FromJson(data["Rotation"]);
		m_Scale = Vec3_FromJson(data["Scale"]);
	}

	TransformComponent* TransformComponent::Clone() const
	{
		auto data = Serialize();
		return new TransformComponent(data["TransformComponent"]);
	}
}