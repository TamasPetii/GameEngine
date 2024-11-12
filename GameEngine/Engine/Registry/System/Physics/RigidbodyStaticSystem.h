#pragma once
#include "EngineApi.h"
#include <Registry/System/System.h>

#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Manager/ResourceManager.h"

#include "Registry/Registry.h"
#include "Registry/Component/Physics/RigidbodyStaticComponent.h"
#include "Registry/Component/Physics/BoxColliderComponent.h"
#include "Registry/Component/Physics/SphereColliderComponent.h"
#include "Registry/Component/Physics/MeshColliderComponent.h"
#include "Registry/Component/Physics/ConvexColliderComponent.h"


class ENGINE_API RigidbodyStaticSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry, PxPhysics* physics, PxScene* scene);
	static void OnUpdate(std::shared_ptr<Registry> registry, PxPhysics* physics, PxScene* scene);
	static void UpdateRigidbodyGlobalPose(std::shared_ptr<Registry> registry, bool init);

	static nlohmann::json Serialize(Registry* registry, Entity entity);
	static void DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data);
};