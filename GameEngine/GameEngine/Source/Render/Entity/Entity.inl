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
    mComponents[typeid(T)] = component;
}