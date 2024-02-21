#pragma once
#include "BillboardRenderer.h"
#include "BloomRenderer.h"
#include "DeferredRenderer.h"
#include "GeometryRenderer.h"
#include "ShadowRenderer.h"
#include "WireframeRenderer.h"
#include "SkyboxRenderer.h"
#include "Scene/Scene.h"

class Renderer
{
public:
	static void RenderScene(std::shared_ptr<Scene> scene);
};