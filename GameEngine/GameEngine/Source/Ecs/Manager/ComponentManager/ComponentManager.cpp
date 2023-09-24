#include "ComponentManager.h"

namespace Ecs
{
	std::unordered_map<Entity*, std::unordered_map<std::type_index, IComponent*>> ComponentManager::m_EntityComponentMap;

	bool ComponentManager::IsEntityRegistered(Entity* entity)
	{
		return m_EntityComponentMap.find(entity) != m_EntityComponentMap.end();
	}

	void ComponentManager::UnRegisterEntity(Entity* entity)
	{
		for (auto [type_index, component] : m_EntityComponentMap[entity])
		{
			delete component;
		}

		m_EntityComponentMap.erase(entity);
	}
}
