#pragma once
#include "EngineApi.h"
#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"

#include "Registry/System/System.h"
#include "Registry/Component/Light/DirLightcomponent.h"
#include "Registry/Component/CameraComponent.h"
#include "Registry/System/CameraSystem.h"

class ENGINE_API DirlightSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);

	static nlohmann::json Serialize(Registry* registry, Entity entity);
	static void DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data);
};

