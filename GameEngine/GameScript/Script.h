#pragma once
#define _USE_MATH_DEFINES 
#include <cmath>
#include <iostream>
#include <filesystem>
#include "../GameEngine/Source/Ecs/Entity/Entity.h"

#define EXPORT_DLL 
#ifdef EXPORT_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

using namespace Ecs;

class DLL_API Script
{
public:
	virtual ~Script() { m_Entity = nullptr; }
	inline void AttachEntity(Ecs::Entity* entity) { m_Entity = entity; }
	virtual void OnStart() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
protected:
	Ecs::Entity* m_Entity;
};

