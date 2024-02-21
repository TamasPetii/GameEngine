#pragma once
#include <memory>
<<<<<<< HEAD
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/DefaultCollider.h"
#include "Registry/Component/SphereCollider.h"
#include "Registry/System/BvhSystem.h"
=======
#include <Registry/Registry.h>
#include <Manager/ResourceManager.h>
#include <Registry/Component/DefaultCollider.h>
#include <Registry/Component/SphereCollider.h>
#include <Registry/System/BvhSystem.h>
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

class WireframeRenderer
{
public:
<<<<<<< HEAD
	static void Render(std::shared_ptr<Registry> registry);
private:
	static void RenderPointLightsVolume(std::shared_ptr<Registry> registry);
	static void RenderSpotLightsVolume(std::shared_ptr<Registry> registry);
	static void RenderDefaultCollider(std::shared_ptr<Registry> registry);
	static void RenderSphereCollider(std::shared_ptr<Registry> registry);
	static void RenderBvhAabb(std::shared_ptr<Registry> registry);
=======
	static void Render(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager);
private:
	static void RenderPointLightsVolume(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager);
	static void RenderSpotLightsVolume(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager);
	static void RenderDefaultCollider(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager);
	static void RenderSphereCollider(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager);
	static void RenderBvhAabb(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager);
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
};

