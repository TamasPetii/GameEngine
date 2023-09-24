#include "ComponentManager.h"

namespace Ecs
{
    template<typename T>
    T* ComponentManager::GetComponent(Entity* entity)
    {
        if (HasComponent<T>(entity))
        {
            return FindComponent<T>(entity);
        }

        return nullptr;
    }

    template<typename T>
    void ComponentManager::AddComponent(Entity* entity, IComponent* component)
    {
        if (!HasComponent<T>(entity))
        {
            m_EntityComponentMap[entity][typeid(T)] = component;
        }
    }

    template<typename ...T>
    bool ComponentManager::HasComponent(Entity* entity)
    {
        if (IsEntityRegistered(entity))
        {
            return (... && (FindComponent<T>(entity) != nullptr));
        }

        return false;
    }

    template<typename T>
    T* ComponentManager::FindComponent(Entity* entity)
    {
        auto component = m_EntityComponentMap[entity].find(typeid(T));

        if (component != m_EntityComponentMap[entity].end() && component->second != nullptr)
        {
            return dynamic_cast<T*>(component->second);
        }

        return nullptr;
    }
}
