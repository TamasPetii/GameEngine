#pragma once
#include <Registry/System/System.h>
#include <Registry/Component/MeshCollider.h>
#include <Registry/Registry.h>
#include <Render/Renderer.h>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Manager/ResourceManager.h>

class MeshColliderSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
	static void OnUpdate(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
};

