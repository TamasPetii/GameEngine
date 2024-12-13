#pragma once
#include "EngineApi.h"
#include <memory>
#include <string>
#include <unordered_set>

class Registry;

class ENGINE_API PreviewRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry, float deltaTime);
	static std::unordered_set<std::string> activeAnimationSet;
protected:
	static void RenderDebugDirlightPreviews(std::shared_ptr<Registry> registry);
	static void RenderShapePreviews(std::shared_ptr<Registry> registry);
	static void RenderModelPreviews(std::shared_ptr<Registry> registry);
	static void RenderMaterialPreviews(std::shared_ptr<Registry> registry);
	static void RenderAnimationPreviews(std::shared_ptr<Registry> registry, float deltaTime);
};