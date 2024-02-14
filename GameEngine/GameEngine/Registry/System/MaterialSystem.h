#pragma once
#include <Registry/System/System.h>
#include <Registry/Registry.h>
#include <Registry/Component/MaterialComponent.h>
#include <Render/Renderer.h>
#include <chrono>
#include <Manager/ResourceManager.h>

class MaterialSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
	static void OnUpdate(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
};

