#pragma once
#include "EngineApi.h"

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
#include "Collision/KdBvh.h"

class ENGINE_API BvhSystem : public System
{
public:
	static int index;
	static KdBvh* bvh;
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
};

