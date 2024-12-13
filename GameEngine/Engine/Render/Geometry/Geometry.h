#pragma once
#include "EngineApi.h"
#include <array>
#include <memory>
#include <vector>
#include <string>
#include <numeric>
#include <glm/glm.hpp>

struct Vertex;
class VertexArrayGL;
class BufferGL;
class ShaderStorageBufferGL;

class ENGINE_API Geometry
{
public:
	Geometry();
	virtual ~Geometry();
	virtual std::string GetName() { return "none"; }

	void Bind();
	void UnBind();
	const auto GetVertexCount() { return m_VertexCount; }
	const auto GetIndexCount() { return m_IndexCount; }
	const auto& GetObb() { return m_Obb; }
	const auto& GetObbOrigin() { return m_ObbOrigin; }
	const auto& GetObbExtents() { return m_ObbExtents; }
	const auto& GetObbMax() { return m_ObbMax; }
	const auto& GetObbMin() { return m_ObbMin; }
	const auto& GetSurfacePoints() { return m_Surfacepoints; }

	void UpdateInstanceSsbo();
	void ClearInstances() { m_Instances.clear(); }
	void AddInstanceID(const glm::uvec4& index) { m_Instances.push_back(index); }
	auto& GetInstanceSsbo() { return m_InstanceSsbo; }
	auto& GetInstances() { return m_Instances; }

	void UpdateShadowInstanceSsbo();
	void ClearShadowInstances() { m_ShadowInstances.clear(); }
	void AddShadowInstanceID(const unsigned int index) { m_ShadowInstances.push_back(index); }
	auto& GetShadowInstanceSsbo() { return m_ShadowInstanceSsbo; }
	auto& GetShadowInstances() { return m_ShadowInstances; }

	const auto& GetVertices() const { return m_Vertices; }
	const auto& GetIndices() const { return m_Indices; }
protected:
	void GenerateObb();
	void GenerateTangents();
	virtual void GenerateBuffers();
	virtual void GenerateVertices() = 0;
	virtual void GenerateIndices() = 0;
	virtual void GenerateSurfacePoints() = 0;
protected: //OBB
	glm::vec3 m_ObbOrigin;
	glm::vec3 m_ObbExtents;
	glm::vec3 m_ObbMax;
	glm::vec3 m_ObbMin;
	std::array<glm::vec3, 8> m_Obb;
protected: //VERTEX DATA
	unsigned int m_VertexCount{ 0 };
	unsigned int m_IndexCount{ 0 };
	std::vector<Vertex> m_Vertices;
	std::vector<glm::vec3> m_Surfacepoints;
	std::vector<glm::uvec4> m_Instances;
	std::vector<unsigned int> m_Indices;
	std::vector<unsigned int> m_ShadowInstances;
protected: //OPENGL RESOURCES
	std::unique_ptr<VertexArrayGL>  m_Vao;
	std::unique_ptr<BufferGL> m_Vbo;
	std::unique_ptr<BufferGL> m_Ibo;
	std::unique_ptr<ShaderStorageBufferGL> m_InstanceSsbo;
	std::unique_ptr<ShaderStorageBufferGL> m_ShadowInstanceSsbo;
};

