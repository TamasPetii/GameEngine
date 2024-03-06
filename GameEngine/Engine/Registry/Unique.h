#pragma once
#include "EngineApi.h"
#include <typeindex>

using UniqueID = unsigned int;

class ENGINE_API Unique
{
public:

    template<typename T>
    static UniqueID typeID() noexcept
    {
        static const UniqueID value = identifier();
        return value;
    }

    template<typename T>
    static std::type_index typeIndex() noexcept
    {
        return typeid(T);
    }

private:
    static UniqueID identifier() noexcept
    {
        static UniqueID value = 0;
        return value++;
    }
};
