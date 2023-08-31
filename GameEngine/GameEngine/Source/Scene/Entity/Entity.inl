#include "Entity.h"

template<typename T>
T* Entity::GetComponent()
{
    auto it = m_Components.find(typeid(T));
    bool success = it != m_Components.end();
    return success ? dynamic_cast<T*>(it->second) : nullptr;
}

template<typename T>
void Entity::AddComponent(T* component)
{
    //static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");

    m_Components[typeid(T)] = dynamic_cast<Component*>(component);
}

template<typename T>
bool Entity::HasComponent()
{
    auto it = m_Components.find(typeid(T));
    return it != m_Components.end();
}