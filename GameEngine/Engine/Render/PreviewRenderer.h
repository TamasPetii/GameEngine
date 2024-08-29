#pragma once
#include "EngineApi.h"
#include <vector>
#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Manager/ModelManager.h"
#include "Manager/PreviewManager.h"
#include "Manager/TextureManager.h"

class ENGINE_API PreviewRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry);
protected:
	static void RenderShapePreviews(std::shared_ptr<Registry> registry);
	static void RenderModelPreviews(std::shared_ptr<Registry> registry);
	static void RenderMaterialPreviews(std::shared_ptr<Registry> registry);
};