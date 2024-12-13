#pragma once
#include "EngineApi.h"
#include "Entity.h"
#include <vector>

using Flag = unsigned char;
const unsigned char NULL_FLAG = 0b00000000;
const unsigned char UPDATE_FLAG = 0b00000001;
const unsigned char CHANGED_FLAG = 0b00000010;
const unsigned char REGENERATE_FLAG = 0b00000100;
const unsigned char TC_SCALECHANGED_FLAG = 0b00001000;
const unsigned char ERROR_FLAG = 0b11111111;

class ENGINE_API PoolBase
{
public:
    virtual ~PoolBase() = default;
    virtual unsigned int GetSize() = 0;
    virtual void AdjustPool(unsigned int size) = 0;
    virtual void AddNewEntity(Entity entity) = 0;
    virtual void RemoveEntity(Entity entity) = 0;
};

template<typename T>
class Pool : public PoolBase
{
public:
    ~Pool() = default;

    virtual void AdjustPool(unsigned int size) override;
    virtual void AddNewEntity(Entity entity) override;
    virtual void RemoveEntity(Entity entity) override;

    unsigned int GetSize() { return m_DenseEntities.size(); }
    auto& GetDenseFlagsArray() { return m_DenseFlags; }
    auto& GetDenseComponentsArray() { return m_DenseComponents; }
    auto& GetDenseEntitiesArray() { return m_DenseEntities; }
    auto& GetSparseEntitiesArray() { return m_SparseEntities; }

    bool  IsFlagSet(Entity entity, const Flag flag);
    Flag& GetFlag(Entity entity);
    void  SetFlag(Entity entity, const Flag flag);
    void  ResFlag(Entity entity, const Flag flag);

    unsigned int GetIndex(Entity entity);
    bool HasComponent(Entity entity);
    T&   GetComponent(Entity entity);
    void AddComponent(Entity entity, const T& component = T{});
    void RemoveComponent(Entity entity);
private:
    std::vector<Flag>   m_DenseFlags;
    std::vector<T>      m_DenseComponents;
    std::vector<Entity> m_DenseEntities;
    std::vector<Index>  m_SparseEntities;
};

template<typename T>
unsigned int Pool<T>::GetIndex(Entity entity)
{
    return m_SparseEntities[entity];
}

template<typename T>
void Pool<T>::AdjustPool(unsigned int size)
{
    m_SparseEntities.resize(size, null);
}

template<typename T>
void Pool<T>::AddNewEntity(Entity entity)
{
    if (entity >= m_SparseEntities.size())
        m_SparseEntities.push_back(null);
}

template<typename T>
void Pool<T>::RemoveEntity(Entity entity)
{
    Pool<T>::RemoveComponent(entity);
}

template<typename T>
inline bool Pool<T>::HasComponent(Entity entity)
{
    return entity < m_SparseEntities.size() && m_SparseEntities[entity] != null;
}

template<typename T>
T& Pool<T>::GetComponent(Entity entity)
{
    if (HasComponent(entity))
        return m_DenseComponents[m_SparseEntities[entity]];

    static T defaultComponent;
    return defaultComponent;
}

template<typename T>
void Pool<T>::AddComponent(Entity entity, const T& component)
{
    if (entity < m_SparseEntities.size() && m_SparseEntities[entity] == null)
    {
        m_SparseEntities[entity] = m_DenseEntities.size();
        m_DenseEntities.push_back(entity);
        m_DenseComponents.emplace_back(component);
        m_DenseFlags.push_back(UPDATE_FLAG | REGENERATE_FLAG);
    }
}

template<typename T>
void Pool<T>::RemoveComponent(Entity entity)
{
    if (HasComponent(entity))
    {
        auto deleteIndex = m_SparseEntities[entity];
        auto swapIndex = m_DenseEntities.size() - 1;
        m_DenseFlags[swapIndex] |= UPDATE_FLAG;
        m_DenseFlags[swapIndex] |= REGENERATE_FLAG;
        m_SparseEntities[m_DenseEntities[swapIndex]] = deleteIndex;
        std::swap(m_DenseEntities[deleteIndex], m_DenseEntities[swapIndex]);
        std::swap(m_DenseComponents[deleteIndex], m_DenseComponents[swapIndex]);
        std::swap(m_DenseFlags[deleteIndex], m_DenseFlags[swapIndex]);
        m_DenseEntities.pop_back();
        m_DenseComponents.pop_back();
        m_DenseFlags.pop_back();
        m_SparseEntities[entity] = null;
    }
}

template<typename T>
Flag& Pool<T>::GetFlag(Entity entity)
{
    if (HasComponent(entity))
    {
        return m_DenseFlags[m_SparseEntities[entity]];
    }

    return ERROR_FLAG;
}

template<typename T>
bool Pool<T>::IsFlagSet(Entity entity, const Flag flag)
{
    return m_DenseFlags[m_SparseEntities[entity]] & flag;
}

template<typename T>
void Pool<T>::SetFlag(Entity entity, const Flag flag)
{
    m_DenseFlags[m_SparseEntities[entity]] |= flag;
}

template<typename T>
void Pool<T>::ResFlag(Entity entity, const Flag flag)
{
    m_DenseFlags[m_SparseEntities[entity]] &= ~flag;
}
