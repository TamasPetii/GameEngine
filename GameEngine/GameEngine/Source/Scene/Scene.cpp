#include "Scene.h"

Scene::Scene()
{
}

Scene::Scene(const json& data)
{
	DeSerialize(object);
}

Scene::~Scene()
{
	//TODO
}

void Scene::AttachEntity(Ecs::Entity* entity)
{
	m_EntityList.push_back(entity);
}

void Scene::DetachEntity(Ecs::Entity* entity)
{
	auto it = std::find(m_EntityList.begin(), m_EntityList.end(), entity);

	if (it != m_EntityList.end())
		m_EntityList.erase(it);
}

void Scene::AddToDelete(Ecs::Entity* entity)
{
	m_ToDelete.push_back(entity);
}

void Scene::AddToCopy(Ecs::Entity* entity)
{
	//auto data = entity->SaveToJson();
	//m_ToCopy.push_back(new Entity(data["Entity"]));
	//m_ToCopy.push_back(new Entity(*entity));
}

void Scene::OnStart()
{
	for (auto entity : m_EntityList)
	{
		Ecs::ScriptSystem::OnStart(entity);
	}
}

void Scene::OnUpdate(float deltaTime)
{


	if (m_IsUpdating)
	{
		for (auto entity : m_EntityList)
		{
			if (entity->HasComponent<ScriptComponent>())
				entity->GetComponent<ScriptComponent>()->OnUpdate(deltaTime);
		}
	}
}

void Scene::EmptyToDelete()
{
	for (auto entity : m_ToDelete)
	{
		DetachEntity(entity);
		Ecs::EntityManager::DeleteEntity(entity);
	}

	m_ToDelete.clear();
}

void Scene::EmtpyToCopy()
{
	/*
	for (auto entity : m_ToCopy)
	{
		AttachEntity(entity);
		m_ActiveEntity = entity;
	}
	m_ToCopy.clear();
	*/
}

bool Scene::IsActive(Entity* entity)
{
	bool active = entity == m_ActiveEntity;

	for (auto child : entity->Get_Children())
	{
		active = active || IsActive(child);
	}

	return active;
}

json Scene::Serialize() const
{

}

void Scene::DeSerialize(const json& data)
{

}


json Scene::SaveToJson() const
{
	json data;
	data["Scene"]["Name"] = m_Name;
	data["Scene"]["Entities"] = json::array();

	for (auto entity : m_EntityList)
	{
		data["Scene"]["Entities"].push_back(entity->SaveToJson());
	}
	
	return data;
}

void Scene::LoadFromJson(const json& object)
{
	m_Name = object["Name"];
	
	for (const auto& entity : object["Entities"])
	{
		AttachEntity(new Entity(entity["Entity"]));
	}
}