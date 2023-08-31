#pragma once
#include <typeindex>
#include <unordered_map>
#include <algorithm>
#include <random>
#include "../../Engine/Engine.h"

class Entity : public JsonObject
{
public:
	static std::unordered_map<unsigned int, Entity*> ALL_ENTITIES;
    json SaveToJson() const override;
    void LoadFromJson(const json& object) override;

    Entity();
    Entity(const json& object);
    ~Entity();

    template<typename T>
    T* GetComponent();
    template<typename T>
    void AddComponent(T* componenet);
    template<typename T>
    bool HasComponent();

    void AttachChild(Entity* entity);
    void DetachChild(Entity* entity);
    /*Getter*/
    const glm::mat4 Get_ParentTransformMatrix() const;
    inline const auto Get_Id() const { return m_Id; }
    inline const auto& Get_Name() const { return m_Name; }
    inline const auto& Get_Parent() const { return m_Parent; }
    inline const auto& Get_Children() const { return m_Children; }
    /*Reference*/
    inline auto& Ref_Id() { return m_Id; }
    inline auto& Ref_Name() { return m_Name; }
    inline auto& Ref_Parent() { return m_Parent; }
    inline auto& Ref_Children() { return m_Children; }
private:
    unsigned int m_Id;
    std::string m_Name;
    Entity* m_Parent;
    std::vector<Entity*> m_Children;
	std::unordered_map<std::type_index, Component*> m_Components;
};

#include "Entity.inl"