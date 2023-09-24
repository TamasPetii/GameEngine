#pragma once
#include <vector>
#include <typeindex>
#include <unordered_map>

#include "../../Entity/Entity.h"

namespace Ecs
{
	struct Relation
	{
		Entity* parent;
		std::vector<Entity*> children;
	};

	class EntityManager
	{
	public:
		static Entity* CreateEntity();
		static void DeleteEntity(Entity* entity);

		static Entity* Parent(Entity* entity);
		static const std::vector<Entity*>& Children(Entity* entity);
		static void RegisterEntity(Entity* entity);
		static void UnRegisterEntity(Entity* entity);
		static void ConnectRelation(Entity* parent, Entity* child);
		static void DisconnectRelation(Entity* child);
	private:
		static bool IsEntityRegistered(Entity* entity);
	private:
		static std::unordered_map<Entity*, Relation> m_EntityRelationMap;
	};
}