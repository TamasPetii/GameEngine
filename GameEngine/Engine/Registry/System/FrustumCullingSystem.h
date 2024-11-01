#pragma once
#include "EngineApi.h"
#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "System.h"
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/Object/ShapeComponent.h"
#include "Registry/Component/Object/ModelComponent.h"
#include "Registry/Component/Physics/DefaultCollider.h"
#include "Registry/Component/Light/PointlightComponent.h"
#include "Registry/Component/Light/SpotlightComponent.h"
#include "Registry/System/CameraSystem.h"
#include "Collision/CollisionTester.h"
#include "Collision/SphereColliderGjk.h"
#include "Collision/ConvexColliderGjk.h"
#include "Collision/AABB.h"

class ENGINE_API FrustumCullingSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
private:
	static void GeometryCulling(std::shared_ptr<Registry> registry, DefaultCollider& cameraFrustum);
	static void PointLightCulling(std::shared_ptr<Registry> registry, DefaultCollider& cameraFrustum);
	static void SpotLightCulling(std::shared_ptr<Registry> registry, DefaultCollider& cameraFrustum);
};

