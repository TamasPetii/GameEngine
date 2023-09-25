#include "Scene.h"

Scene::Scene()
{
}

Scene::Scene(const json& data)
{
	DeSerialize(data);
}

Scene::~Scene()
{
	for (auto& entity : m_EntityList)
	{
		delete entity;
	}

	m_ActiveEntity = nullptr;
	m_EntityList.clear();
	m_ToDelete.clear();
	m_ToCopy.clear();
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
	m_ToCopy.push_back(entity->Clone());
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
	EmptyToDelete();
	EmptyToCopy();

	if (m_IsUpdating)
	{
		for (auto entity : m_EntityList)
		{
			if (entity->HasComponent<ScriptComponent>())
				Ecs::ScriptSystem::OnUpdate(entity, deltaTime);
		}
	}
}

void Scene::EmptyToDelete()
{
	for (auto entity : m_ToDelete)
	{
		DetachEntity(entity);
		delete entity;
	}

	m_ToDelete.clear();
}

void Scene::EmptyToCopy()
{
	for (auto entity : m_ToCopy)
	{
		AttachEntity(entity);
		m_ActiveEntity = entity;
	}

	m_ToCopy.clear();
}

bool Scene::IsActive(Ecs::Entity* entity)
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
	json data;
	data["Scene"]["Entities"] = json::array();

	for (auto entity : m_EntityList)
	{
		data["Scene"]["Entities"].push_back(entity->Serialize());
	}

	return data;
}

void Scene::DeSerialize(const json& data)
{
	for (const auto& entity : data["Entities"])
	{
		AttachEntity(new Ecs::Entity(entity["Entity"], false));
	}
}