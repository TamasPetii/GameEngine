#pragma once
#include "EngineApi.h"
#include <Registry/System/System.h>

#include <algorithm>
#include <execution>
#include <glm/glm.hpp>

#include "Manager/ResourceManager.h"

#include "Registry/Registry.h"
#include "Registry/Component/SphereColliderComponent.h"
#include "Registry/Component/ShapeComponent.h"
#include "Registry/Component/ModelComponent.h"

using namespace physx;

class ENGINE_API SphereColliderSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);

	static nlohmann::json Serialize(Registry* registry, Entity entity);
	static void DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data);
};
