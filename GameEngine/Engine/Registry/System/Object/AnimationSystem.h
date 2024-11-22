#pragma once
#include "EngineApi.h"
#include <algorithm>
#include <execution>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"

#include "Registry/System/System.h"
#include "Registry/Component/Object/AnimationComponent.h"

class ENGINE_API AnimationSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry, float deltaTime);
	static void OnEnd(std::shared_ptr<Registry> registry);
	static void CalculateBoneTransforms(AnimationComponent& animationComponent, float deltaTime);

	static nlohmann::json Serialize(Registry* registry, Entity entity);
	static void DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data);
};

