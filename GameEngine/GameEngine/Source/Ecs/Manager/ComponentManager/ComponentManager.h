#pragma once
#include <typeindex>
#include <unordered_map>

#include "../IManager.h"
#include "../../Entity/Entity.h"
#include "../../Component/Components.h"

namespace Ecs
{
    class ComponentManager : public IManager
    {
    public:
        static void UnRegisterEntity(Entity* entity);

        template<typename T>
        static T* GetComponent(Entity* entity);
        template<typename T>
        static void AddComponent(Entity* entity, IComponent* component);
        template<typename ...T>
        static bool HasComponent(Entity* entity);
    private:
        template<typename T>
        static T* FindComponent(Entity* entity);
        static bool IsEntityRegistered(Entity* entity);
    private:
        static std::unordered_map<Entity*, std::unordered_map<std::type_index, IComponent*>> m_EntityComponentMap;
    };
}

#include "ComponentManager.inl"
