#pragma once
#include "EngineApi.h"
#include "Registry/System/System.h"
#include "Registry/Entity.h"
#include <memory>

class Registry;

class ENGINE_API ModelSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
	static void OnEnd(std::shared_ptr<Registry> registry);

	static nlohmann::json Serialize(Registry* registry, Entity entity);
	static void DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data);
};

