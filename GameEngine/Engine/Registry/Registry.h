#pragma once
#include "EngineApi.h"
#include <typeindex>
#include <deque>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <nlohmann/json.hpp>

#include "Pool.h"
#include "Entity.h"
#include "Unique.h"

class ENGINE_API Registry
{
public:
    Registry();
    Registry(nlohmann::json& data);
    ~Registry();

    Entity CreateEntity();
    void DestroyEntity(Entity entity);
    void SetActiveEntity(Entity entity) { m_ActiveEntity = entity; }
    const auto& GetActiveEntity() { return m_ActiveEntity; };
    const auto& GetActiveEntities() { return m_ActiveEntities; }
    const auto& GetInactiveEntities() { return m_InactiveEntities; }
    const auto& GetEntityCount() { return m_NextEntity; }
    const auto& GetParents() { return m_Parents; }
    const auto& GetChildren() { return m_Children; }
    void SetParent(Entity entity, Parent parent);
    bool IsDeepConnected(Entity entityA, Entity entityB);
    bool HasParent(Entity entity) { return m_Parents[entity] != null; }
    nlohmann::json Serialize();
    void DeSerialize(nlohmann::json& data);

    template<typename T>
    Pool<T>* GetComponentPool();
    template<typename T>
    bool HasComponent(Entity entity);
    template<typename T>
    T& GetComponent(Entity entity);
    template<typename T>
    void AddComponent(Entity entity, const T& component = T{});
    template<typename T>
    void RemoveComponent(Entity entity);
    template<typename T>
    void SetFlag(Entity entity, const Flag flag);
    template<typename T>
    unsigned int GetIndex(Entity entity);
    template<typename T>
    unsigned int GetSize();

    void DeSerializeEntity(nlohmann::json& data);
    nlohmann::json SerializeEntity(Entity entity);
private:
    Entity m_NextEntity{ 0 };
    Entity m_ActiveEntity = null;
    std::vector<Entity> m_ActiveEntities;
    std::vector<Parent> m_Parents;
    std::vector<std::vector<Child>> m_Children;
    std::deque<Entity>  m_InactiveEntities;
    std::unordered_map<std::type_index, PoolBase*> m_Pools;
};

//UniqueID typeID = Unique::typeID<T>();
//std::unordered_map<UniqueID, PoolBase*> m_Pools;

template<typename T>
Pool<T>* Registry::GetComponentPool()
{
    std::type_index typeID = Unique::typeIndex<T>();

    if (m_Pools.find(typeID) != m_Pools.end())
        return static_cast<Pool<T>*>(m_Pools[typeID]);

    return nullptr;
}

template<typename T>
void Registry::AddComponent(Entity entity, const T& component)
{
    std::type_index typeID = Unique::typeIndex<T>();

    if (m_Pools.find(typeID) == m_Pools.end())
    {
        PoolBase* pool = new Pool<T>{};
        pool->AdjustPool(m_NextEntity);
        m_Pools.insert(std::make_pair(typeID, pool));
    }

    auto pool = static_cast<Pool<T>*>(m_Pools[typeID]);
    pool->AddComponent(entity, component);
}

template<typename T>
bool Registry::HasComponent(Entity entity)
{
    std::type_index typeID = Unique::typeIndex<T>();
    return m_Pools.find(typeID) != m_Pools.end() && static_cast<Pool<T>*>(m_Pools[typeID])->HasComponent(entity);
}

template<typename T>
T& Registry::GetComponent(Entity entity)
{
    std::type_index typeID = Unique::typeIndex<T>();
    if (m_Pools.find(typeID) != m_Pools.end())
        return static_cast<Pool<T>*>(m_Pools[typeID])->GetComponent(entity);

    static T component;
    return component;
}

template<typename T>
void Registry::RemoveComponent(Entity entity)
{
    std::type_index typeID = Unique::typeIndex<T>();
    if (m_Pools.find(typeID) != m_Pools.end())
        static_cast<Pool<T>*>(m_Pools[typeID])->RemoveComponent(entity);
}

template<typename T>
void Registry::SetFlag(Entity entity, const Flag flag)
{
    std::type_index typeID = Unique::typeIndex<T>();
    auto pool = static_cast<Pool<T>*>(m_Pools[typeID]);
    pool->SetFlag(entity, flag);
}

template<typename T>
unsigned int Registry::GetIndex(Entity entity)
{
    std::type_index typeID = Unique::typeIndex<T>();
    auto pool = static_cast<Pool<T>*>(m_Pools[typeID]);
    return pool->GetIndex(entity);
}

template<typename T>
unsigned int Registry::GetSize()
{
    std::type_index typeID = Unique::typeIndex<T>();
    auto pool = static_cast<Pool<T>*>(m_Pools[typeID]);
    return pool->GetDenseEntitiesArray().size();
}