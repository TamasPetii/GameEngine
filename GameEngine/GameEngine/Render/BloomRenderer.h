#pragma once
#include <memory>
<<<<<<< HEAD
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
=======
#include <Registry/Registry.h>
#include <Manager/ResourceManager.h>
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

class BloomRenderer
{
public:
<<<<<<< HEAD
	static void Render(std::shared_ptr<Registry> registry);
=======
	static void Render(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager);
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	static inline bool  useBloom = true;
	static inline float gamma = 1.25f;
	static inline float exposure = 1.0f;
private:
<<<<<<< HEAD
	static void RenderBloom(std::shared_ptr<Registry> registry);
	static void RenderBloomUp(std::shared_ptr<Registry> registry);
	static void RenderBloomDown(std::shared_ptr<Registry> registry);
=======
	static void RenderBloom(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager);
	static void RenderBloomUp(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager);
	static void RenderBloomDown(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> resourceManager);
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
};

