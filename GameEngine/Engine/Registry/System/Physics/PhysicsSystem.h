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
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);

	static nlohmann::json Serialize(Registry* registry, Entity entity);
	static void DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data);
};

