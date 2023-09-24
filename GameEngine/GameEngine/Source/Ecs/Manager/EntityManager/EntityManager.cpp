#include "EntityManager.h"

namespace Ecs
{
	std::unordered_map<Entity*, Relation> EntityManager::m_EntityRelationMap;

	Entity* EntityManager::CreateEntity()
	{
		Entity* entity = new Entity();
		RegisterEntity(entity);
	}

	void EntityManager::DeleteEntity(Entity* entity)
	{
		UnRegisterEntity(entity);
		delete entity;
	}

	Entity* EntityManager::Parent(Entity* entity)
	{
		if (IsEntityRegistered(entity))
		{
			return m_EntityRelationMap[entity].parent;
		}

		return nullptr;
	}

	const std::vector<Entity*>& EntityManager::Children(Entity* entity)
	{
		if (IsEntityRegistered(entity))
		{
			return m_EntityRelationMap[entity].children;
		}

		return std::vector<Entity*>();
	}

	void EntityManager::RegisterEntity(Entity* entity)
	{
		if (!IsEntityRegistered(entity))
		{
			m_EntityRelationMap[entity];
		}
	}

	void EntityManager::UnRegisterEntity(Entity* entity)
	{
		if (IsEntityRegistered(entity))
		{
			DisconnectRelation(entity);
			m_EntityRelationMap.erase(entity);
		}
	}

	void EntityManager::ConnectRelation(Entity* parent, Entity* child)
	{
		/*
		- Disconnect child and child's parent from each other 
		- Register child as parent's child
		- Register parent as child's parent
		*/
		DisconnectRelation(child);
		m_EntityRelationMap[parent].children.push_back(child);
		m_EntityRelationMap[child].parent = parent;
	}

	void EntityManager::DisconnectRelation(Entity* child)
	{
		/*
		- Check if entity has parent
		- UnRegister child from parent's children
		- UnRegister parent from child's parent
		*/

		auto parent = m_EntityRelationMap[child].parent;

		if (parent != nullptr)
		{
			auto begin = m_EntityRelationMap[parent].children.begin();
			auto end = m_EntityRelationMap[parent].children.end();
			auto it = std::find(begin, end, child);
			m_EntityRelationMap[parent].children.erase(it);
			m_EntityRelationMap[child].parent = nullptr;
		}
	}

	bool EntityManager::IsEntityRegistered(Entity* entity)
	{
		return m_EntityRelationMap.find(entity) != m_EntityRelationMap.end();
	}

}
