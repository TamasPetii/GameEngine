#pragma once
<<<<<<< HEAD
#include <queue>
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
=======
#pragma once
#include <Registry/System/System.h>
#include <Registry/Component/DefaultCollider.h>
#include <Registry/Registry.h>
#include <Render/Renderer.h>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Manager/ResourceManager.h>
#include <Collision/Bvh.h>
#include <Collision/BvhStatic.h>
#include <queue>
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

class BvhSystem : public System
{
public:
	static inline int index = 0;
	static inline Bvh* bvh = nullptr;
	static inline BvhStatic* bvhStatic = nullptr;
<<<<<<< HEAD
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
=======
	static void OnStart(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
	static void OnUpdate(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
};

