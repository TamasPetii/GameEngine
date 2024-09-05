#pragma once
#include "EngineApi.h"
#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "System.h"
#include "Registry/Registry.h"
#include "Render/OpenGL/Camera.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/ShapeComponent.h"
#include "Registry/Component/ModelComponent.h"
#include "Registry/Component/DefaultCollider.h"
#include "Registry/Component/PointLightComponent.h"
#include "Collision/CollisionTester.h"
#include "Collision/SphereColliderGjk.h"
#include "Collision/ConvexColliderGjk.h"

class ENGINE_API ShadowCullingSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
private:
	static void PointLightShadowCulling(std::shared_ptr<Registry> registry);
	static void SpotLightShadowCulling(std::shared_ptr<Registry> registry);
};

