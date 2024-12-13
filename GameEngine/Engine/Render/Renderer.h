#pragma once
#include "EngineApi.h"
#include <memory>
#include <typeindex>
#include <unordered_map>

class Registry;

class ENGINE_API Renderer
{
public:
	static void RenderScene(std::shared_ptr<Registry> registry, float deltaTime);
	static std::unordered_map<std::type_index, double> m_RenderTimes;
	static std::unordered_map<std::type_index, double> m_AverageRenderTimes;
};
