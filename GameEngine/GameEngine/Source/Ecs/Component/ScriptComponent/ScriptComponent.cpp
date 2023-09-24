#include "ScriptComponent.h"

namespace Ecs
{
	ScriptComponent::ScriptComponent()
	{

	}

	ScriptComponent::ScriptComponent(const json& data)
	{
		DeSerialize(data);
	}

	json ScriptComponent::Serialize() const
	{
		json data;

		data["ScriptComponent"]["Scripts"] = json::array();

		for (auto [name, script] : m_Scripts)
		{
			data["ScriptComponent"]["Scripts"].push_back(name);
		}

		return data;
	}

	void ScriptComponent::DeSerialize(const json& data)
	{
		for (auto name : data["Scripts"])
		{
			m_Scripts.insert(std::pair<std::string, Script*>(name, nullptr));
		}
	}

	ScriptComponent* ScriptComponent::Clone() const
	{
		auto data = Serialize();
		return new ScriptComponent(data["ScriptComponent"]);
	}

}
