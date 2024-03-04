#pragma once
#include "EngineApi.h"
#include <Registry/Component/MaterialComponent.h>
#include <Render/OpenGL/Vertex.h>
#include <Render/OpenGL/BufferGL.h>
#include <Render/OpenGL/VertexArrayGL.h>
#include <Render/OpenGL/ShaderStorageBufferGL.h>
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include <queue>
#include "AssimpConverter.h"
#include <Manager/TextureManager.h>

class ENGINE_API Model
{
public:
	Model();
	~Model();
	void Load(const std::string& path);
	void Bind();
	void UnBind();
	auto GetIndexCount() { return m_IndexCount; }
	auto GetVertexCount() { return m_VertexCount; }
	auto GetMeshCount() { return m_MeshCount; }
	auto GetInstanceCount() { return m_Instances.size(); }
	auto& GetMaterialSsbo() { return m_MaterialSsbo; }
	auto& GetInstanceSsbo() { return m_InstanceSsbo; }
	void ClearInstances() { m_Instances.clear(); }
	void AddInstanceID(const glm::uvec4& ids) { m_Instances.push_back(ids); }
	void UpdateInstanceSsbo();
private:
	void GenerateBuffers();
	void PreProcess(aiNode* node, const aiScene* scene);
	void Process(aiNode* node, const aiScene* scene);
	void ProcessGeometry(aiMesh* mesh, const aiScene* scene, unsigned int& count);
	std::string m_Path;
	std::string m_Directory;
	GLuint m_MeshCount{0};
	GLuint m_IndexCount{0};
	GLuint m_VertexCount{0};
	std::vector<std::string> m_MeshName;
	std::unique_ptr<VertexArrayGL> m_Vao;
	std::unique_ptr<IndexBufferGL> m_Ibo;
	std::unique_ptr<VertexBufferGL> m_Vbo;
	std::vector<GLuint> m_Indices;
	std::vector<Vertex> m_Vertices;
	std::vector<glm::uvec4> m_Instances;
	std::vector<MaterialComponent> m_Materials;
	std::unique_ptr<ShaderStorageBufferGL> m_MaterialSsbo;
	std::unique_ptr<ShaderStorageBufferGL> m_InstanceSsbo;
};

