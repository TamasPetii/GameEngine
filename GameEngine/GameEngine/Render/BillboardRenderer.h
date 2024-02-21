#pragma once
#include <memory>
<<<<<<< HEAD
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Manager/TextureManager.h"
=======
#include <Registry/Registry.h>
#include <Manager/ResourceManager.h>
#include <Manager/AssetManager.h>
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

class BillboardRenderer
{
public:
<<<<<<< HEAD
	static void Render(std::shared_ptr<Registry> registry);
protected:
	static void RenderDirLightBillboard(std::shared_ptr<Registry> registry);
	static void RenderPointLightBillboard(std::shared_ptr<Registry> registry);
	static void RenderSpotLightBillboard(std::shared_ptr<Registry> registry);
	static void RenderAudioBillboard(std::shared_ptr<Registry> registry);
=======
	static void Render(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resouceManager, std::shared_ptr<AssetManager> assetManager);
protected:
	static void RenderDirLightBillboard(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<AssetManager> assetManager);
	static void RenderPointLightBillboard(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<AssetManager> assetManager);
	static void RenderSpotLightBillboard(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<AssetManager> assetManager);
	static void RenderAudioBillboard(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<AssetManager> assetManager);
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
};

