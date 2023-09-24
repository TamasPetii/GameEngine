#include "TagComponent.h"

namespace Ecs
{
	TagComponent::TagComponent() :
		m_Tag("Empty")
	{
	}

	TagComponent::TagComponent(const json& data)
	{
		DeSerialize(data);
	}

	json TagComponent::Serialize() const
	{
		json data;
		data["TagComponent"]["Tag"] = m_Tag;
		return data;
	}

	void TagComponent::DeSerialize(const json& data)
	{
		m_Tag = data["Tag"];
	}

	TagComponent* TagComponent::Clone() const
	{
		auto data = Serialize();
		return new TagComponent(data["TagComponent"]);
	}
}