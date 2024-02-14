#pragma once
#include <Registry/System/System.h>
#include <Registry/Registry.h>
#include <Render/Renderer.h>
#include <Registry/Component/TransformComponent.h>
#include <Manager/ResourceManager.h>

class TransformSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
	static void OnUpdate(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
	static void OnEnd(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
};

