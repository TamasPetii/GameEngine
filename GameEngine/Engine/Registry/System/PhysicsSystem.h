#pragma once
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

class PhysicsSystem : public System
{
public:
	static inline glm::vec3 gravity{ 0, -9.81, 0 };
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry, float deltaTime);
	static void OnEnd(std::shared_ptr<Registry> registry);
};
