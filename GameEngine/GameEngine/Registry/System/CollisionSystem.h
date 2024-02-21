#pragma once
<<<<<<< HEAD
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
#include "Collision/Bvh.h"
#include "Collision/BvhStatic.h"
#include "Collision/Simplex.h"
#include "Collision/CollisionTester.h"
#include "BvhSystem.h"
=======
#include <Registry/System/System.h>
#include <Registry/Component/DefaultCollider.h>
#include <Registry/Registry.h>
#include <Render/Renderer.h>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Manager/ResourceManager.h>
#include <Collision/Bvh.h>
#include <atomic>
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

class CollisionSystem : public System
{
public:
<<<<<<< HEAD
	static inline int naiveCounter = 0;
	static inline std::atomic<int> aabbCounter = 0;
	static inline std::atomic<int> gjkCounter = 0;
	static inline std::atomic<int> gjkSuccess = 0;

	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
	static inline std::vector<std::tuple<Entity, Entity, Simplex>> collisionData;
=======
	static void OnStart(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
	static void OnUpdate(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
};

