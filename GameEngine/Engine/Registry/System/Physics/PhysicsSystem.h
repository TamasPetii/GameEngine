#pragma once
#include "EngineApi.h"
#include "Registry/System/System.h"
#include <glm/glm.hpp>
#include <physx/PxPhysicsAPI.h>
using namespace physx;

class ENGINE_API PhysicsSystem : public System
{
public:
	static void OnUpdate(PxScene* gScene, float deltaTime);
	static nlohmann::json Serialize();
	static void DeSerialize(const nlohmann::json& data);
	static glm::vec3 gravity;
};

