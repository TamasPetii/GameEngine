#include "Entity.h"

std::unordered_map<unsigned int, Entity*> Entity::ALL_ENTITIES;

Entity::Entity(const Entity& other) 
	: m_Parent(nullptr)
{
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<unsigned int> dis;

	m_Id = dis(gen);
	m_Name = "Entity##" + std::to_string(m_Id);

	for (auto component : other.m_Components)
	{
		Component* c = component.second->Clone();

		if (auto meshComponent = dynamic_cast<MeshComponent*>(c))
		{
			AddComponent(meshComponent);
		}

		if (auto transfromComponent = dynamic_cast<TransformComponent*>(c))
		{
			AddComponent(transfromComponent);
		}

		if (auto lightComponent = dynamic_cast<LightComponent*>(c))
		{
			AddComponent(lightComponent);
		}

		if (auto skyComponent = dynamic_cast<SkyComponent*>(c))
		{
			AddComponent(skyComponent);
		}

	}

	std::cout << (HasComponent<MeshComponent>() ? "MeshComponent: True" : "MeshComponent: False") << std::endl;

	for (auto child : other.m_Children)
	{
		Entity* newChild = new Entity(*child);
		AttachChild(newChild);
	}

	ALL_ENTITIES[m_Id] = this;
}

Entity::Entity()
{
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<unsigned int> dis;

	m_Id = dis(gen);
	m_Name = "Entity##" + std::to_string(m_Id);
	ALL_ENTITIES[m_Id] = this;
}

Entity::Entity(const json& object)
{
	LoadFromJson(object);
	ALL_ENTITIES[m_Id] = this;
}

Entity::~Entity()
{
	if (m_Parent != nullptr)
		m_Parent->DetachChild(this);

	for (auto& child : m_Children)
	{
		delete child;
	}

	for (auto& component : m_Components)
	{
		delete component.second;
	}

	m_Children.clear();
	m_Components.clear();	
	ALL_ENTITIES.erase(m_Id);
}

void Entity::AttachChild(Entity* entity)
{
	entity->Ref_Parent() = this;
	m_Children.push_back(entity);
}

void Entity::DetachChild(Entity* entity)
{
	auto it = std::find(m_Children.begin(), m_Children.end(), entity);

	if (it != m_Children.end())
	{
		entity->Ref_Parent() = nullptr;
		m_Children.erase(it);
	}
}

const glm::mat4 Entity::Get_ParentTransformMatrix() const
{
	if (m_Parent != nullptr)
		return m_Parent->Get_ParentTransformMatrix() * m_Parent->GetComponent<TransformComponent>()->Get_TransformMatrix();
	return glm::mat4(1);
}

json Entity::SaveToJson() const
{
	json data;
	data["Entity"]["Id"] = m_Id;
	data["Entity"]["Name"] = m_Name;
	data["Entity"]["Components"] = json::array();
	data["Entity"]["Children"] = json::array();

	for (auto [type, component] : m_Components)
	{
		data["Entity"]["Components"].push_back(component->SaveToJson());
	}

	for (auto child : m_Children)
	{
		data["Entity"]["Children"].push_back(child->SaveToJson());
	}

	return data;
}

void Entity::LoadFromJson(const json& object)
{
	m_Id = object["Id"];
	m_Name = object["Name"];
	
	for (auto component : object["Components"])
	{
		if (component.find("TransformComponent") != component.end())
			AddComponent(new TransformComponent(component["TransformComponent"]));

		if (component.find("MeshComponent") != component.end())
			AddComponent(new MeshComponent(component["MeshComponent"]));

		if (component.find("LightComponent") != component.end())
			AddComponent(new LightComponent(component["LightComponent"]));
	}

	for (auto child : object["Children"])
	{
		auto entityChild = new Entity(child["Entity"]);
		entityChild->Ref_Parent() = this;
		this->AttachChild(entityChild);
	}
}