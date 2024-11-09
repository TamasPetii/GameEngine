#pragma once
#include "EngineApi.h"

#include <memory>
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Manager/TextureManager.h"
#include "Registry/Component/AudioComponent.h"
#include "Settings/GlobalSettings.h"
#include "Registry/System/CameraSystem.h"

class ENGINE_API BillboardRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry);
	static float iconSize;
protected:
	static void RenderDirLightBillboard(std::shared_ptr<Registry> registry);
	static void RenderPointLightBillboard(std::shared_ptr<Registry> registry);
	static void RenderSpotLightBillboard(std::shared_ptr<Registry> registry);
	static void RenderAudioBillboard(std::shared_ptr<Registry> registry);
	static void RenderCameraBillboard(std::shared_ptr<Registry> registry);
};

