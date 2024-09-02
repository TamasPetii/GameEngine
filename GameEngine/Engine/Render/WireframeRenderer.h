#pragma once
#include <memory>

#include "EngineApi.h"
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/DirlightComponent.h"
#include "Registry/Component/DefaultCollider.h"
#include "Registry/Component/SphereColliderComponent.h"
#include "Registry/Component/BoxColliderComponent.h"
#include "Settings/GlobalSettings.h"

class ENGINE_API WireframeRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry);
	static bool ShowDirLightsLines;
	static bool ShowPointLightsVolume;
	static bool ShowSpotLightsVolume;
	static bool ShowDefaultCollider;
	static bool ShowBoxCollider;
	static bool ShowSphereCollider;
	static bool ShowBvhBoxes;
private:
	static void RenderDirLightsLine(std::shared_ptr<Registry> registry);
	static void RenderPointLightsVolume(std::shared_ptr<Registry> registry);
	static void RenderSpotLightsVolume(std::shared_ptr<Registry> registry);
	static void RenderDefaultCollider(std::shared_ptr<Registry> registry);
	static void RenderBoxCollider(std::shared_ptr<Registry> registry);
	static void RenderSphereCollider(std::shared_ptr<Registry> registry);
	static void RenderBvhAabb(std::shared_ptr<Registry> registry);
};

