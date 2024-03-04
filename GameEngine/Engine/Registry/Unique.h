#pragma once

using UniqueID = unsigned int;

class Unique
{
public:
    template<typename T>
    static UniqueID typeID() noexcept
    {
        static const UniqueID value = identifier();
        return value;
    }
private:
    static UniqueID identifier() noexcept
    {
        static UniqueID value = 0;
        return value++;
    }
};
