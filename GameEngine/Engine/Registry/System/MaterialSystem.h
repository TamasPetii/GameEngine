#pragma once
#include "EngineApi.h"
#include <algorithm>
#include <execution>
#include <glm/glm.hpp>

#include "System.h"
#include "Registry/Registry.h"
#include "Registry/Component/MaterialComponent.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/ModelComponent.h"
#include "Registry/Component/ShapeComponent.h"

class ENGINE_API MaterialSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);

	static nlohmann::json Serialize(Registry* registry, Entity entity);
	static void DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data);
};
