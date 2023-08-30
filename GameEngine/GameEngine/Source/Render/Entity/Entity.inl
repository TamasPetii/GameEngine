#include "Entity.h"

template<typename T>
T* Entity::GetComponent()
{
    auto it = mComponents.find(typeid(T));
    bool success = it != mComponents.end();
    return success ? dynamic_cast<T*>(it->second) : nullptr;
}

template<typename T>
void Entity::AddComponent(T* component)
{
    mComponents[typeid(T)] = dynamic_cast<Component*>(component);
}

template<typename T>
bool Entity::HasComponent()
{
    auto it = mComponents.find(typeid(T));
    return it != mComponents.end();
}