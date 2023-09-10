#include "Scene.h"

Scene::Scene()
{
	m_Name = "MainScene";
}

Scene::Scene(const json& object)
{
	LoadFromJson(object);
}

Scene::~Scene()
{

}

void Scene::AttachEntity(Entity* entity)
{
	m_EntityList.push_back(entity);
}

void Scene::DetachEntity(Entity* entity)
{
	auto it = std::find(m_EntityList.begin(), m_EntityList.end(), entity);

	if (it != m_EntityList.end())
		m_EntityList.erase(it);
}

void Scene::AddToDelete(Entity* entity)
{
	m_ToDelete.push_back(entity);
}

void Scene::AddToCopy(Entity* entity)
{
	//auto data = entity->SaveToJson();
	//m_ToCopy.push_back(new Entity(data["Entity"]));
	m_ToCopy.push_back(new Entity(*entity));
}

void Scene::OnStart()
{
	for (auto entity : m_EntityList)
	{
		if (entity->HasComponent<ScriptComponent>())
			entity->GetComponent<ScriptComponent>()->OnStart();
	}
}

void Scene::OnUpdate(float deltaTime)
{
	for (auto entity : m_ToDelete)
	{
		DetachEntity(entity);
		delete entity;
	}
	m_ToDelete.clear();

	for (auto entity : m_ToCopy)
	{
		AttachEntity(entity);
		m_ActiveEntity = entity;
	}
	m_ToCopy.clear();

	for (auto entity : m_EntityList)
	{
		if (entity->HasComponent<ScriptComponent>())
			entity->GetComponent<ScriptComponent>()->OnUpdate(deltaTime);
	}
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