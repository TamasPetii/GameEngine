#pragma once
#include "EngineApi.h"
#include <memory>
#include <nlohmann/json.hpp>

class Registry;

class ENGINE_API BloomRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry);
	static nlohmann::json Serialize();
	static void DeSerialize(const nlohmann::json& data);
public:
	static bool  useBloom;
	static float gamma;
	static float exposure;
private:
	static void RenderBloom(std::shared_ptr<Registry> registry);
	static void RenderBloomUp(std::shared_ptr<Registry> registry);
	static void RenderBloomDown(std::shared_ptr<Registry> registry);
};

