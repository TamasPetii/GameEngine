#pragma once
#include <memory>
#include <Registry/Registry.h>
#include <Manager/ResourceManager.h>
#include <Manager/AssetManager.h>

class BillboardRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager, std::shared_ptr<AssetManager> assetManager);
protected:
	static void RenderDirLightBillboard(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<AssetManager> assetManager);
	static void RenderPointLightBillboard(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<AssetManager> assetManager);
	static void RenderSpotLightBillboard(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<AssetManager> assetManager);
	static void RenderAudioBillboard(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<AssetManager> assetManager);
};

