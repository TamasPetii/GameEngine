#pragma once
#include "EngineApi.h"
#include "Registry/Entity.h"
#include <memory>

class Registry;

class ENGINE_API Script
{
public:
	Script(std::shared_ptr<Registry> registry, Entity entity);
	virtual ~Script();
	virtual void OnStart() {}
	virtual void OnUpdate(float deltaTime) {}
	virtual void OnGui() {}
	virtual void OnCollisionEnter(Entity collidedEntity) {}
	virtual void OnCollisionExit(Entity collidedEntity) {}
	virtual void OnTriggerEnter(Entity collidedEntity) {}
	virtual void OnTriggerExit(Entity collidedEntity) {}
protected:
	Entity entity;
	std::shared_ptr<Registry> registry;
};