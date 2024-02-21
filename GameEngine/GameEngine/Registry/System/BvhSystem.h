#pragma once
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

class BvhSystem : public System
{
public:
	static inline int index = 0;
	static inline Bvh* bvh = nullptr;
	static inline BvhStatic* bvhStatic = nullptr;
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
};

