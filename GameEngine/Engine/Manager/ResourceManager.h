#pragma once
#include "EngineApi.h"
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

class ENGINE_API ResourceManager
{
public:
	static ResourceManager* Instance();
	static void Destroy();

	auto GetUbo(const std::string& name) { return m_UniformBuffers[name]; }
	auto GetSsbo(const std::string& name) { return m_ShaderStorageBuffers[name]; }
	auto GetProgram(const std::string& name) { return m_Programs[name]; }
	auto GetGeometry(const std::string& name) { return m_Geometries[name]; }
	auto GetFbo(const std::string& name) { return m_FrameBuffers[name]; }
	auto& GetGeometryList() { return m_Geometries; }
private:
	static inline ResourceManager* m_Instance = nullptr;
	ResourceManager();
	void Init();
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

