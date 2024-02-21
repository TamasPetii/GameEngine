#pragma once
<<<<<<< HEAD
#include "BillboardRenderer.h"
#include "BloomRenderer.h"
#include "DeferredRenderer.h"
#include "GeometryRenderer.h"
#include "ShadowRenderer.h"
#include "WireframeRenderer.h"
#include "SkyboxRenderer.h"
#include "Scene/Scene.h"
=======

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <random>
#include <string>
#include <unordered_map>
#include <execution>
#include <filesystem>

#include <Render/OpenGL/ProgramGL.h>
#include <Render/OpenGL/Camera.h>
#include <Render/OpenGL/ShaderStorageBufferGL.h>
#include <Render/OpenGL/UniformBufferGL.h>
#include <Render/OpenGL/Vertex.h>
#include <Render/OpenGL/VertexArrayGL.h>
#include <Render/OpenGL/TextureGL.h>
#include <Render/OpenGL/FramebufferGL.h>
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

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
#include <Manager/AssetManager.h>

#include <Render/DeferredRenderer.h>
#include <Render/BillboardRenderer.h>
#include <Render/WireframeRenderer.h>
#include <Render/BloomRenderer.h>

#include <Collision/CollisionTester.h>

class Renderer
{
public:
<<<<<<< HEAD
	static void RenderScene(std::shared_ptr<Scene> scene);
};
=======
	static Renderer* Instance();
	static void Destroy();
	~Renderer();
	void Render();
	void Update();

	std::shared_ptr<Camera> m_Camera;
	std::shared_ptr<Registry> m_Registry;
	std::shared_ptr<AssetManager> m_AssetManager;
	std::shared_ptr<ResourceManager> m_ResourceManager;
private:
	Renderer();
	static Renderer* m_Instance;

	void InitTexture();
public:
	void InitRegistry();
};

inline Renderer* Renderer::m_Instance = nullptr;
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
