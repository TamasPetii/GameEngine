#pragma once
#include "EngineApi.h"
#include <Registry/System/System.h>

#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/Physics/DefaultCollider.h"
#include "Registry/Component/Object/ModelComponent.h"

class ENGINE_API DefaultColliderSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
};

