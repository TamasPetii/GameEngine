#pragma once
#include "EngineApi.h"

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
#include "Collision/KdBvh.h"
#include "Collision/Simplex.h"
#include "Collision/CollisionTester.h"
#include "BvhSystem.h"

class ENGINE_API CollisionSystem : public System
{
public:
	static int naiveCounter;
	static std::atomic<int> aabbCounter;
	static std::atomic<int> gjkCounter;
	static std::atomic<int> gjkSuccess;

	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
	static std::vector<std::tuple<Entity, Entity, Simplex>> collisionData;
};

