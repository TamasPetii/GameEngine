#pragma once
#include "EngineApi.h"

#include <memory>
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"

class ENGINE_API BloomRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry);
	static inline bool  useBloom = true;
	static inline float gamma = 1.25f;
	static inline float exposure = 1.0f;
private:
	static void RenderBloom(std::shared_ptr<Registry> registry);
	static void RenderBloomUp(std::shared_ptr<Registry> registry);
	static void RenderBloomDown(std::shared_ptr<Registry> registry);
};

