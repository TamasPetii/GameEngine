#pragma once
#include "EngineApi.h"
#include <Registry/System/System.h>

#include <algorithm>
#include <execution>

#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"

class ENGINE_API PhysicsSystem : public System
{
public:
	static void OnUpdate(physx::PxScene* gScene, float deltaTime);
	static glm::vec3 gravity;


	static nlohmann::json Serialize();
	static void DeSerialize(const nlohmann::json& data);
};

