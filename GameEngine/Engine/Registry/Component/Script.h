#pragma once
#include "EngineApi.h"
#include "Registry/Registry.h"

class ENGINE_API Script
{
public:
	Script(std::shared_ptr<Registry> registry, Entity entity) : registry(registry), entity(entity) {}
	virtual ~Script() = default;
	virtual void OnStart() {};
	virtual void OnUpdate(float deltaTime) {};
	virtual void OnGui() {}
protected:
	Entity entity;
	std::shared_ptr<Registry> registry;
};