#pragma once
<<<<<<< HEAD
#include <memory>
#include "Registry/Registry.h"
=======
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

class ShadowRenderer
{
public:
<<<<<<< HEAD
	static void Render(std::shared_ptr<Registry> registry);
private:
	static void RenderDirLightShadows(std::shared_ptr<Registry> registry);
	static void RenderPointLightShadows(std::shared_ptr<Registry> registry);
	static void RenderSpotLightShadows(std::shared_ptr<Registry> registry);
=======
private:
	static void RenderDirLightShadows();
	static void RenderPointLightShadows();
	static void RenderSpotLightShadows();
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
};

