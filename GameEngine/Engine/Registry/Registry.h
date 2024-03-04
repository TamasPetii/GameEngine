#pragma once
#include "EngineApi.h"
#include <typeindex>
#include <deque>
#include <vector>
#include <unordered_map>

#include "Pool.h"
#include "Entity.h"
#include "Unique.h"

class ENGINE_API Registry
{
public:
    Entity CreateEntity();
    void DestroyEntity(Entity entity);
    void SetActiveEntity(Entity entity) { m_ActiveEntity = entity; }
    const auto& GetActiveEntity() { return m_ActiveEntity; };
    const auto& GetActiveEntities() { return m_ActiveEntities; }
    const auto& GetInactiveEntities() { return m_InactiveEntities; }
    const auto& GetParents() { return m_Parents; }
    const auto& GetChildren() { return m_Children; }
    void SetParent(Entity entity, Parent parent);
    bool IsDeepConnected(Entity entityA, Entity entityB);
    bool HasParent(Entity entity) { return m_Parents[entity] != null; }

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
private:
    Entity m_NextEntity{ 0 };
    Entity m_ActiveEntity = null;
    std::vector<Entity> m_ActiveEntities;
    std::vector<Parent> m_Parents;
    std::vector<std::vector<Child>> m_Children;
    std::deque<Entity>  m_InactiveEntities;
    //std::unordered_map<UniqueID, PoolBase*> m_Pools;
    std::unordered_map<std::type_index, PoolBase*> m_Pools;
};

template<typename T>
Pool<T>* Registry::GetComponentPool()
{
    //UniqueID typeID = Unique::typeID<T>();
    std::type_index typeID = typeid(T);

    if (m_Pools.find(typeID) != m_Pools.end())
        return static_cast<Pool<T>*>(m_Pools[typeID]);

    return nullptr;
}

template<typename T>
void Registry::AddComponent(Entity entity, const T& component)
{
    //UniqueID typeID = Unique::typeID<T>();
    std::type_index typeID = typeid(T);

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
    //UniqueID typeID = Unique::typeID<T>();
    std::type_index typeID = typeid(T);
    return m_Pools.find(typeID) != m_Pools.end() && static_cast<Pool<T>*>(m_Pools[typeID])->HasComponent(entity);
}

template<typename T>
T& Registry::GetComponent(Entity entity)
{
    //UniqueID typeID = Unique::typeID<T>();
    std::type_index typeID = typeid(T);
    if (m_Pools.find(typeID) != m_Pools.end())
        return static_cast<Pool<T>*>(m_Pools[typeID])->GetComponent(entity);

    exit(1);
}

template<typename T>
void Registry::RemoveComponent(Entity entity)
{
    //UniqueID typeID = Unique::typeID<T>();
    std::type_index typeID = typeid(T);
    if (m_Pools.find(typeID) != m_Pools.end())
        static_cast<Pool<T>*>(m_Pools[typeID])->RemoveComponent(entity);
}

inline Entity Registry::CreateEntity()
{
    Entity newEntity{ null };

    if (m_InactiveEntities.size() != 0)
    {
        newEntity = m_InactiveEntities.front();
        m_InactiveEntities.pop_front();
    }
    else
    {
        newEntity = m_NextEntity++;
        m_Parents.push_back(null);
        m_Children.push_back(std::vector<Child>{});
    }

    for (auto& [typeID, pool] : m_Pools)
        pool->AddNewEntity(newEntity);

    m_ActiveEntities.push_back(newEntity);
    return newEntity;
}

inline void Registry::DestroyEntity(Entity entity)
{
    for (auto& [typeID, pool] : m_Pools)
        pool->RemoveEntity(entity);

    auto it = std::find(m_ActiveEntities.begin(), m_ActiveEntities.end(), entity);

    m_ActiveEntities.erase(it);
    m_InactiveEntities.push_front(entity);
}

template<typename T>
void Registry::SetFlag(Entity entity, const Flag flag)
{
    //UniqueID typeID = Unique::typeID<T>();
    std::type_index typeID = typeid(T);
    auto pool = static_cast<Pool<T>*>(m_Pools[typeID]);
    pool->SetFlag(entity, flag);
}

template<typename T>
unsigned int Registry::GetIndex(Entity entity)
{
    //UniqueID typeID = Unique::typeID<T>();
    std::type_index typeID = typeid(T);
    auto pool = static_cast<Pool<T>*>(m_Pools[typeID]);
    return pool->GetIndex(entity);
}

template<typename T>
unsigned int Registry::GetSize()
{
    //UniqueID typeID = Unique::typeID<T>();
    std::type_index typeID = typeid(T);
    auto pool = static_cast<Pool<T>*>(m_Pools[typeID]);
    return pool->GetDenseEntitiesArray().size();
}

inline void Registry::SetParent(Entity entity, Parent parent)
{
    if (m_Parents[entity] != null)
    {
        auto& children = m_Children[m_Parents[entity]];
        auto it = std::find(children.begin(), children.end(), entity);
        children.erase(it);
        m_Parents[entity] = null;
    }

    m_Parents[entity] = parent;
    if(parent != null)
        m_Children[parent].push_back(entity);
}

inline bool Registry::IsDeepConnected(Entity entityA, Entity entityB)
{
    while (m_Parents[entityB] != null)
    {
        if (m_Parents[entityB] == entityA)
            return true;

        entityB = m_Parents[entityB];
    }

    return false;
}