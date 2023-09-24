#pragma once
#include <string>
#include <random>

namespace Ecs
{
    class Entity
    {
    public:
        Entity();
        /* <<Getter>> */
        inline const auto& Get_Id() const { return m_Id; }
        /* <<Reference>> */
        inline auto& Ref_Id() { return m_Id; }
    private:
        unsigned int m_Id;
    };
}
