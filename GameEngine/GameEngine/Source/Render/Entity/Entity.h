#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <typeindex>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include "../../Engine/Engine.h"

class Entity
{
public:
    Entity();
    ~Entity();
    static Entity* Get(int id);

    void AddChild(Entity* entity);
    void Remove();
    void RemoveChild(Entity* entity);

    template<typename T>
    T* GetComponent();

    template<typename T>
    void AddComponent(T* componenet);

    template<typename T>
    bool HasComponent();

    //Getter
    inline int& GetId() { return mId; }
    inline std::string& GetText() { return mText; }
    inline Entity*& GetParent() { return mParent; }
    inline std::vector<Entity*>& GetChildren() { return mChildren; }
    glm::mat4 GetParentTransformMatrix();
    static std::unordered_map<int, Entity*> mEntities;
private:
    int mId;
    std::string mText;
	std::unordered_map<std::type_index, Component*> mComponents;


    Entity* mParent;
    std::vector<Entity*> mChildren;
};

#include "Entity.inl"
