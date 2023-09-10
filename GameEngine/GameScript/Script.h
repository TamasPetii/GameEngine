#pragma once
#define _USE_MATH_DEFINES 
#include <cmath>
#include <iostream>
#include <filesystem>
#include "../GameEngine/Source/Scene/Entity/Entity.h"

#define EXPORT_DLL 
#ifdef EXPORT_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

class DLL_API Script
{
public:
	virtual ~Script() { m_Entity = nullptr; }
	void AttachEntity(Entity* entity) { m_Entity = entity; }
	virtual void OnStart() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
protected:
	Entity* m_Entity;
};

