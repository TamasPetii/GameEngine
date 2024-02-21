#pragma once
#include <string>
#include <random>
#include <vector>
#include <typeindex>
#include <unordered_map>
#include "../Component/Components.h"
#include "../../ISerializable.h"
#include "../../IClonable.h"

namespace Ecs
{
    class Entity : public ISerializable, public ICloneable
    {
    public:
        Entity();
        Entity(const json& data, bool generateId);
        ~Entity();
        json Serialize() const override;
        void DeSerialize(const json& data) override;
        Entity* Clone() const override;

        //Components
        template<typename T>
        T* GetComponent();
        template<typename T>
        void AddComponent(IComponent* component);
        template<typename ...T>
        bool HasComponent();

        //Children Parent Relation
        void Connect(Entity* entity);
        void Disconnect(Entity* entity);
        static Entity* Find(unsigned int id);

        /* <<Getter>> */
        inline const auto& Get_Id() const { return m_Id; }
        inline const auto& Get_Parent() const { return m_Parent; }
        inline const auto& Get_Children() const { return m_Children; }
        /* <<Reference>> */
        inline auto& Ref_Id() { return m_Id; }
        inline auto& Ref_Parent() { return m_Parent; }
        inline auto& Ref_Children() { return m_Children; }
    private:
        template<typename T>
        T* FindComponent();
        static unsigned int GenerateId();
    private:
        unsigned int m_Id;
        Entity* m_Parent;
        std::vector<Entity*> m_Children;
        std::unordered_map<std::type_index, IComponent*> m_ComponentMap;
        static std::unordered_map<unsigned int, Entity*> m_AllEntities;
    };
}

#include "Entity.inl"