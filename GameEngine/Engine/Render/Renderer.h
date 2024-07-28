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
#include <Registry/System/MaterialSystem.h>
#include <Registry/System/TransformSystem.h>
#include <Registry/System/DirlightSystem.h>
#include <Registry/System/PointLightSystem.h>
#include <Registry/System/SpotLightSystem.h>
#include <Registry/System/DefaultColliderSystem.h>
#include <Registry/System/SphereColliderSystem.h>
#include <Registry/System/MeshColliderSystem.h>
#include <Registry/System/FrustumCullingSystem.h>
#include <Registry/System/InstanceSystem.h>
#include <Registry/System/BvhSystem.h>
#include <Registry/System/CollisionSystem.h>

#include <Registry/Component/TransformComponent.h>
#include <Registry/Component/MaterialComponent.h>
#include <Registry/Component/DirlightComponent.h>
#include <Registry/Component/PointlightComponent.h>
#include <Registry/Component/ShapeComponent.h>
#include <Registry/Component/SpotLightComponent.h>
#include <Registry/Component/DefaultCollider.h>
#include <Registry/Component/SphereCollider.h>
#include <Registry/Component/MeshCollider.h>

#include <Geometry/Cube.h>
#include <Geometry/Sphere.h>
#include <Geometry/Pyramid.h>
#include <Geometry/Cylinder.h>
#include <Geometry/Torus.h>

#include <Manager/ResourceManager.h>

#include <Render/DeferredRenderer.h>
#include <Render/BillboardRenderer.h>
#include <Render/WireframeRenderer.h>
#include <Render/BloomRenderer.h>
#include <Render/WaterRenderer.h>

#include <Collision/CollisionTester.h>
#include <Settings/GlobalSettings.h>

class ENGINE_API Renderer
{
public:
	static void RenderScene(std::shared_ptr<Scene> scene, float deltaTime);
	static std::unordered_map<std::type_index, double> m_RenderTimes;
	static std::unordered_map<std::type_index, double> m_AverageRenderTimes;
};
