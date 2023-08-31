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

void Scene::DeleteEntity(Entity* entity)
{
	m_ToDelete.push_back(entity);
}

void Scene::OnStart()
{

}

void Scene::OnUpdate()
{
	for (auto entity : m_ToDelete)
	{
		DetachEntity(entity);
		delete entity;
	}
	m_ToDelete.clear();
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