#pragma once
#include "EngineApi.h"
#include <algorithm>
#include <execution>
#include <glm/glm.hpp>

#include "System.h"
#include "Registry/Registry.h"
#include "Registry/Component/TransformComponent.h"
#include "Registry/Component/AudioComponent.h"
#include "Manager/ResourceManager.h"
#include "Manager/SoundManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

class ENGINE_API AudioSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
	static void OnEnd(std::shared_ptr<Registry> registry);

	static nlohmann::json Serialize(Registry* registry, Entity entity);
	static void DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data);
};

