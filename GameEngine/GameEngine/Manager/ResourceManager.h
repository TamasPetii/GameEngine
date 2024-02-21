#pragma once
#include <string>
#include <unordered_map>
#include <Render/OpenGL/ProgramGL.h>
#include <Render/OpenGL/FramebufferGL.h>
#include <Render/OpenGL/UniformBufferGL.h>
#include <Render/OpenGL/ShaderStorageBufferGL.h>
#include <Geometry/Geometry.h>
#include <Geometry/Cube.h>
#include <Geometry/Sphere.h>
#include <Geometry/Pyramid.h>
#include <Geometry/Cylinder.h>
#include <Geometry/Torus.h>
#include <Geometry/Cone.h>
#include <Registry/Component/TransformComponent.h>
#include <Registry/Component/MaterialComponent.h>
#include <Registry/Component/DirlightComponent.h>
#include <Registry/Component/PointlightComponent.h>
#include <Registry/Component/SpotlightComponent.h>
#include <Registry/Component/ShapeComponent.h>

class ResourceManager
{
public:
<<<<<<< HEAD
	static ResourceManager* Instance();
	static void Destroy();

=======
	void Init();
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	auto GetUbo(const std::string& name) { return m_UniformBuffers[name]; }
	auto GetSsbo(const std::string& name) { return m_ShaderStorageBuffers[name]; }
	auto GetProgram(const std::string& name) { return m_Programs[name]; }
	auto GetGeometry(const std::string& name) { return m_Geometries[name]; }
	auto GetFbo(const std::string& name) { return m_FrameBuffers[name]; }
<<<<<<< HEAD
	auto& GetGeometryList() { return m_Geometries; }
private:
	static inline ResourceManager* m_Instance = nullptr;
	ResourceManager();
	void Init();
=======
private:
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	void InitPrograms();
	void InitGeometries();
	void InitFrameBuffers();
	void InitUniformBuffers();
	void InitShaderStorageBuffers();
	std::unordered_map<std::string, std::shared_ptr<ProgramGL>> m_Programs;
	std::unordered_map<std::string, std::shared_ptr<Geometry>> m_Geometries;
	std::unordered_map<std::string, std::shared_ptr<FramebufferGL>> m_FrameBuffers;
	std::unordered_map<std::string, std::shared_ptr<UniformBufferGL>> m_UniformBuffers;
	std::unordered_map<std::string, std::shared_ptr<ShaderStorageBufferGL>> m_ShaderStorageBuffers;
};
