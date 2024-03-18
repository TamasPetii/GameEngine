#pragma once
#include "EngineApi.h"
#include <typeindex>

using UniqueID = unsigned int;

class ENGINE_API Unique
{
public:
    template<typename T>
    static UniqueID typeID();
    template<typename T>
    static std::type_index typeIndex();
private:
    static UniqueID identifier();
};

template<typename T>
UniqueID Unique::typeID()
{
    static const UniqueID value = identifier();
    return value;
}

template<typename T>
std::type_index Unique::typeIndex()
{
    return typeid(T);
}