#pragma once
#include <mutex>
#include <tuple>
#include <queue>
#include <atomic>
#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "System.h"
#include "Registry/Registry.h"
#include "Render/OpenGL/Camera.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/DefaultCollider.h"
#include "Registry/Component/RigidbodyComponent.h"
#include "Collision/Simplex.h"
#include "Collision/CollisionTester.h"
#include <Collision/CollisionResponder.h>
#include "BvhSystem.h"
#include "CollisionSystem.h"

class CollisionResponseSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
};

