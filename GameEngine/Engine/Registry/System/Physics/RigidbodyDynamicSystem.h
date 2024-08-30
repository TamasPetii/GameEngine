#pragma once
#include "EngineApi.h"
#include <Registry/System/System.h>

#include <algorithm>
#include <execution>
#include <glm/glm.hpp>

#include "Manager/ResourceManager.h"

#include "Registry/Registry.h"
#include "Registry/Component/RigidbodyDynamicComponent.h"
#include "Registry/Component/BoxColliderComponent.h"
#include "Registry/Component/SphereColliderComponent.h"
#include "Registry/Component/MeshColliderComponent.h"
#include "Registry/Component/ConvexColliderComponent.h"

class ENGINE_API RigidbodyDynamicSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry, PxPhysics* physics, PxScene* scene);
	static void OnUpdate(std::shared_ptr<Registry> registry, PxPhysics* physics, PxScene* scene);

	static nlohmann::json Serialize(Registry* registry, Entity entity);
	static void DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data);
};