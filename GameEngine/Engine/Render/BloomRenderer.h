#pragma once
#include "EngineApi.h"

#include <memory>
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"

class ENGINE_API BloomRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry);
	static bool  useBloom;
	static float gamma;
	static float exposure;
private:
	static void RenderBloom(std::shared_ptr<Registry> registry);
	static void RenderBloomUp(std::shared_ptr<Registry> registry);
	static void RenderBloomDown(std::shared_ptr<Registry> registry);
};

