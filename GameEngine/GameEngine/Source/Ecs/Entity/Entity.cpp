#include "Entity.h"

namespace Ecs
{
	std::unordered_map<unsigned int, Entity*> Entity::m_AllEntities;

	Entity* Entity::Find(unsigned int id)
	{
		if (m_AllEntities.find(id) != m_AllEntities.end())
		{
			return m_AllEntities[id];
		}
	}

	unsigned int Entity::GenerateId()
	{
		static std::random_device rnd;
		static std::mt19937 gen(rnd());
		static std::uniform_int_distribution<unsigned int> dist;

		unsigned int id = 0;

		do
		{
			 id = dist(gen);
		} 
		while (id == 0);

		return id;
	}

	Entity::Entity() :
		m_Id(0),
		m_Parent(nullptr)
	{
		m_Id = GenerateId();
		m_AllEntities[m_Id] = this;
	}

	Entity::Entity(const json& data, bool generateId)
	{
		DeSerialize(data);

		if (generateId)
		{
			m_Id = GenerateId();
		}

		m_AllEntities[m_Id] = this;
	}

	Entity::~Entity()
	{
		Disconnect(this);

		for (auto& [type, component] : m_ComponentMap)
		{
			delete component;
		}

		for (auto& child : m_Children)
		{
			delete child;
		}

		m_ComponentMap.clear();
		m_Children.clear();
		m_AllEntities.erase(m_Id);
	}

	json Entity::Serialize() const
	{
		json data;
		data["Entity"]["Id"] = m_Id;
		data["Entity"]["Components"] = json::array();
		data["Entity"]["Children"] = json::array();

		for (auto [type, component] : m_ComponentMap)
		{
			data["Entity"]["Components"].push_back(component->Serialize());
		}

		for (auto child : m_Children)
		{
			data["Entity"]["Children"].push_back(child->Serialize());
		}

		return data;
	}

	void Entity::DeSerialize(const json& data)
	{
		m_Id = data["Id"];

		for (auto component : data["Components"])
		{
			if (component.find("TransformComponent") != component.end())
				AddComponent<TransformComponent>(new TransformComponent(component["TransformComponent"]));

			else if (component.find("MeshComponent") != component.end())
				AddComponent<MeshComponent>(new MeshComponent(component["MeshComponent"]));

			else if (component.find("LightComponent") != component.end())
				AddComponent<LightComponent>(new LightComponent(component["LightComponent"]));

			else if (component.find("ScriptComponent") != component.end())
				AddComponent<ScriptComponent>(new ScriptComponent(component["ScriptComponent"]));

			else if (component.find("TagComponent") != component.end())
				AddComponent<TagComponent>(new TagComponent(component["TagComponent"]));

			else if (component.find("SkyboxComponent") != component.end())
				AddComponent<SkyboxComponent>(new SkyboxComponent(component["SkyboxComponent"]));

			else if (component.find("RenderComponent") != component.end())
				AddComponent<RenderComponent>(new RenderComponent(component["RenderComponent"]));
		}

		for (auto child : data["Children"])
		{
			auto newChild = new Entity(child["Entity"], false);
			Connect(newChild);
		}
	}

	Entity* Entity::Clone() const
	{
		auto data = Serialize();
		return new Entity(data["Entity"], true);
	}

	void Entity::Connect(Entity* entity)
	{
		/*
		- Disconnect child and it's parent from each other
		- Register entity as child's parent
		- Register child as entity's child
		*/

		Disconnect(entity);
		entity->Ref_Parent() = this;
		m_Children.push_back(entity);
	}

	void Entity::Disconnect(Entity* entity)
	{
		auto parent = entity->Ref_Parent();

		if (parent != nullptr)
		{
			auto begin = parent->Get_Children().begin();
			auto end = parent->Get_Children().end();
			auto it = std::find(begin, end, entity);
			parent->Ref_Children().erase(it);
			entity->Ref_Parent() = nullptr;
		}
	}
}