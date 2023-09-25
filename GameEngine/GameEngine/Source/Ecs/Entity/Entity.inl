#include "Entity.h"

namespace Ecs
{
    template<typename T>
    T* Entity::GetComponent()
    {
        if (HasComponent<T>())
        {
            return FindComponent<T>();
        }

        return nullptr;
    }

    template<typename T>
    void Entity::AddComponent(IComponent* component)
    {
        if (!HasComponent<T>())
        {
            m_ComponentMap[typeid(T)] = component;
        }
    }

    template<typename ...T>
    bool Entity::HasComponent()
    {
        return (... && (FindComponent<T>() != nullptr));
    }

    template<typename T>
    T* Entity::FindComponent()
    {
        auto component = m_ComponentMap.find(typeid(T));

        if (component != m_ComponentMap.end() && component->second != nullptr)
        {
            return dynamic_cast<T*>(component->second);
        }

        return nullptr;
    }
}
