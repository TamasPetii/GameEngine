#pragma once
#include <memory>
#include <Registry/Registry.h>
#include <Manager/ResourceManager.h>
#include <Registry/Component/DefaultCollider.h>
#include <Registry/Component/SphereCollider.h>
#include <Registry/System/BvhSystem.h>

class WireframeRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager);
private:
	static void RenderPointLightsVolume(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager);
	static void RenderSpotLightsVolume(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager);
	static void RenderDefaultCollider(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager);
	static void RenderSphereCollider(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager);
	static void RenderBvhAabb(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager);
};

