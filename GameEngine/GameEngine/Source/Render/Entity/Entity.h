#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <typeindex>
#include "../../Engine/Engine.h"

class Entity
{
public:
	Entity();
    ~Entity();

    inline int& GetId() { return mId; }
    inline std::string& GetText() { return mText; }

    template<typename T>
    T* GetComponent();

    template<typename T>
    void AddComponent(T* componenet);

private:
    int mId;
    std::string mText;
	std::unordered_map<std::type_index, Component*> mComponents;
};

#include "Entity.inl"
