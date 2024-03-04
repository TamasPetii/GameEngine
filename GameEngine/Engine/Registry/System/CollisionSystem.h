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
	static inline int naiveCounter = 0;
	static inline std::atomic<int> aabbCounter = 0;
	static inline std::atomic<int> gjkCounter = 0;
	static inline std::atomic<int> gjkSuccess = 0;

	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
	static inline std::vector<std::tuple<Entity, Entity, Simplex>> collisionData;
};

