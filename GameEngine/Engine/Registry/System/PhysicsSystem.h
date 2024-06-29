#pragma once
#include "EngineApi.h"
#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "System.h"
#include "Registry/Registry.h"
#include "Registry/Component/TransformComponent.h"
#include "Registry/Component/RigidbodyComponent.h"
#include "Manager/ResourceManager.h"

class ENGINE_API PhysicsSystem : public System
{
public:
	static glm::vec3 gravity;
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry, float deltaTime);
	static void OnEnd(std::shared_ptr<Registry> registry);

	static nlohmann::json Serialize(Registry* registry, Entity entity);
	static void DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data);
};
