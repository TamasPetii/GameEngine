#pragma once
#define _USE_MATH_DEFINES 
#include <cmath>
#include <iostream>
#include <filesystem>
#include <memory>
#include "Registry/Registry.h"
#include "Registry/Component/Components.h"

#define EXPORT_DLL 
#ifdef EXPORT_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

class DLL_API BaseScript
{
public:
	virtual ~BaseScript() = default;
	virtual void OnStart() = 0;
	virtual void OnUpdate(float deltaTime) = 0;

	inline void InitScript(std::shared_ptr<Registry> registry, Entity entity)
	{
		this->registry = registry;
		this->entity = entity;
	}
protected:
	Entity entity;
	std::shared_ptr<Registry> registry;
};

