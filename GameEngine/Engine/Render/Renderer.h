#pragma once
#include "EngineApi.h"

#include "BillboardRenderer.h"
#include "BloomRenderer.h"
#include "DeferredRenderer.h"
#include "GeometryRenderer.h"
#include "ShadowRenderer.h"
#include "WireframeRenderer.h"
#include "SkyboxRenderer.h"
#include "Scene/Scene.h"

#include <Registry/Registry.h>

#include "Geometry/Cube.h"
#include "Geometry/Sphere.h"
#include "Geometry/Pyramid.h"
#include "Geometry/Cylinder.h"
#include "Geometry/Torus.h"

#include <Manager/ResourceManager.h>

#include <Render/DeferredRenderer.h>
#include <Render/BillboardRenderer.h>
#include <Render/WireframeRenderer.h>
#include <Render/BloomRenderer.h>
#include <Render/WaterRenderer.h>
#include <Render/PreviewRenderer.h>

#include <Collision/CollisionTester.h>
#include <Settings/GlobalSettings.h>

class ENGINE_API Renderer
{
public:
	static void RenderScene(std::shared_ptr<Scene> scene, float deltaTime);
	static std::unordered_map<std::type_index, double> m_RenderTimes;
	static std::unordered_map<std::type_index, double> m_AverageRenderTimes;
};
