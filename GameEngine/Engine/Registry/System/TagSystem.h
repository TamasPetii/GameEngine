#pragma once
#include "EngineApi.h"
#include <algorithm>
#include <execution>

#include "System.h"
#include "Registry/Registry.h"
#include "Registry/Component/TagComponent.h"
#include "Manager/ResourceManager.h"

class ENGINE_API TagSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
	static void OnEnd(std::shared_ptr<Registry> registry);

	static nlohmann::json Serialize(Registry* registry, Entity entity);
	static void DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data);
};

