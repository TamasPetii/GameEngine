#pragma once
#include "EngineApi.h"
#include "Registry/System/System.h"
#include "Registry/Entity.h"
#include <physx/PxPhysicsAPI.h>
using namespace physx;

class Registry;

class ENGINE_API RigidbodyStaticSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry, PxPhysics* physics, PxScene* scene);
	static void OnUpdate(std::shared_ptr<Registry> registry, PxPhysics* physics, PxScene* scene);
	static void UpdateRigidbodyGlobalPose(std::shared_ptr<Registry> registry, bool init);

	static nlohmann::json Serialize(Registry* registry, Entity entity);
	static void DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data);
};