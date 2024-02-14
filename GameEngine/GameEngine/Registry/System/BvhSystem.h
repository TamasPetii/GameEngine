#pragma once
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

class BvhSystem : public System
{
public:
	static inline int index = 0;
	static inline Bvh* bvh = nullptr;
	static inline BvhStatic* bvhStatic = nullptr;
	static void OnStart(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
	static void OnUpdate(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
};

