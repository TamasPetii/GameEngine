#pragma once
#include "EngineApi.h"
#include <Registry/System/System.h>

#include <algorithm>
#include <execution>
#include <glm/glm.hpp>

#include "Manager/ResourceManager.h"
#include "Manager/ModelManager.h"

#include "Registry/Registry.h"
#include "Registry/Component/MeshColliderComponent.h"
#include "Registry/Component/ShapeComponent.h"
#include "Registry/Component/ModelComponent.h"

class ENGINE_API MeshColliderSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry, PxPhysics* physics);
	static void OnUpdate(std::shared_ptr<Registry> registry, PxPhysics* physics);

	static nlohmann::json Serialize(Registry* registry, Entity entity);
	static void DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data);
};
