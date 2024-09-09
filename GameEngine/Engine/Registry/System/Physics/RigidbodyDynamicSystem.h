#pragma once
#include "EngineApi.h"
#include <Registry/System/System.h>

#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Manager/ResourceManager.h"

#include "Registry/Registry.h"
#include "Registry/Component/Physics/RigidbodyDynamicComponent.h"
#include "Registry/Component/Physics/BoxColliderComponent.h"
#include "Registry/Component/Physics/SphereColliderComponent.h"
#include "Registry/Component/Physics/MeshColliderComponent.h"
#include "Registry/Component/Physics/ConvexColliderComponent.h"

class ENGINE_API RigidbodyDynamicSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry, PxPhysics* physics, PxScene* scene);
	static void OnUpdate(std::shared_ptr<Registry> registry, PxPhysics* physics, PxScene* scene);
	static void UpdateRigidbodyGlobalPose(std::shared_ptr<Registry> registry);
	static void FetchRigidbodyGlobalPose(std::shared_ptr<Registry> registry);

	static nlohmann::json Serialize(Registry* registry, Entity entity);
	static void DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data);
};