#pragma once
#include "EngineApi.h"
#include <memory>

class Registry;

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
	static bool ShowCameraVolume;
private:
	static void RenderDirLightsLine(std::shared_ptr<Registry> registry);
	static void RenderPointLightsVolume(std::shared_ptr<Registry> registry);
	static void RenderSpotLightsVolume(std::shared_ptr<Registry> registry);
	static void RenderDefaultCollider(std::shared_ptr<Registry> registry);
	static void RenderBoxCollider(std::shared_ptr<Registry> registry);
	static void RenderSphereCollider(std::shared_ptr<Registry> registry);
	static void RenderCameraVolume(std::shared_ptr<Registry> registry);
};

