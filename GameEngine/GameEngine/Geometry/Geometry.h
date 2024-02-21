#pragma once
#include <memory>
#include <numeric>
#include <array>
#include <iostream>
#include <Render/OpenGL/BufferGL.h>
#include <Render/OpenGL/VertexArrayGL.h>
#include <Render/OpenGL/Vertex.h>
#include <Render/OpenGL/ShaderStorageBufferGL.h>

class Geometry
{
public:
	Geometry();
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
	const auto& GetInstances() { return m_Instances; }
	auto GetInstanceSsbo() { return m_InstanceSsbo; }
	void ClearInstances() { m_Instances.clear(); }
	void AddInstanceID(const glm::uvec4& index) { m_Instances.push_back(index); }
	void UpdateInstanceUbo();
protected:
	void GenerateObb();
	void GenerateTangents();
	virtual void GenerateBuffers();
	virtual void GenerateVertices() = 0;
	virtual void GenerateIndices() = 0;
	virtual void GenerateSurfacePoints() = 0;
	glm::vec3 m_ObbOrigin;
	glm::vec3 m_ObbExtents;
	glm::vec3 m_ObbMax;
	glm::vec3 m_ObbMin;
	GLuint m_VertexCount{ 0 };
	GLuint m_IndexCount{ 0 };
	std::array<glm::vec3, 8> m_Obb;
	std::vector<GLuint> m_Indices;
	std::vector<Vertex> m_Vertices;
	std::vector<glm::vec3> m_Surfacepoints;
	std::unique_ptr<VertexArrayGL>  m_Vao;
	std::unique_ptr<VertexBufferGL> m_Vbo;
	std::unique_ptr<IndexBufferGL>  m_Ibo;
	std::shared_ptr<ShaderStorageBufferGL> m_InstanceSsbo;
	std::vector<glm::uvec4> m_Instances;
};

