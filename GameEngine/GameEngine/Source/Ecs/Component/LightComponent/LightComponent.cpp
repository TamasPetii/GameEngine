#include "LightComponent.h"

namespace Ecs
{
	LightComponent::LightComponent() :
		m_LightSource(nullptr)
	{
	}

	LightComponent::LightComponent(const json& data)
	{
		DeSerialize(data);
	}

	LightComponent::~LightComponent()
	{
		delete m_LightSource;
	}


	json LightComponent::Serialize() const
	{
		json data;

		if (m_LightSource != nullptr)
		{
			data["LightComponent"]["LightSource"] = m_LightSource->Serialize();
		}
		else
		{
			data["LightComponent"]["LightSource"] = nullptr;
		}

		return data;
	}

	void LightComponent::DeSerialize(const json& data)
	{
		auto lightSource = data["LightSource"];

		if (lightSource.find("DirectionLight") != lightSource.end())
		{
			m_LightSource = new DirectionLight(lightSource["DirectionLight"]);
		}
		else if (lightSource.find("PointLight") != lightSource.end())
		{
			m_LightSource = new PointLight(lightSource["PointLight"]);
		}
		else
		{
			m_LightSource = nullptr;
		}
	}

	LightComponent* LightComponent::Clone() const
	{
		auto data = Serialize();
		return new LightComponent(data["LightComponent"]);
	}
}