#pragma once
#include "EngineApi.h"
#include <memory>
#include <vector>
#include <string>
#include <array>
#include <unordered_map>
#include <glm/glm.hpp>
#include <assimp/types.h>
#include <Assimp/scene.h>

struct Vertex;
class BufferGL;
class TextureGL;
class VertexArrayGL;
class ShaderStorageBufferGL;
struct MaterialComponent;

class ENGINE_API Model
{
public:
	Model();
	virtual ~Model();
	bool Load(const std::string& path);

	void Bind();
	void UnBind();
	const auto& GetPath() const { return m_Path; }
	auto GetIndexCount() { return m_IndexCount; }
	auto GetVertexCount() { return m_VertexCount; }
	auto GetMeshCount() { return m_MeshCount; }
	auto& GetMaterialSsbo() { return m_MaterialSsbo; }
	std::vector<glm::vec3> m_VertexPositions;
	std::vector<unsigned int> m_VertexIndices;

	const auto& GetObb() { return m_Obb; }
	const auto& GetObbOrigin() { return m_ObbOrigin; }
	const auto& GetObbExtents() { return m_ObbExtents; }
	const auto& GetObbMax() { return m_ObbMax; }
	const auto& GetObbMin() { return m_ObbMin; }

	void UpdateInstanceSsbo();
	void ClearInstances() { m_Instances.clear(); }
	void AddInstanceID(const glm::uvec4& ids) { m_Instances.push_back(ids); }
	auto& GetInstances() { return m_Instances; }
	auto& GetInstanceSsbo() { return m_InstanceSsbo; }

	void UpdateShadowInstanceSsbo();
	void ClearShadowInstances() { m_ShadowInstances.clear(); }
	void AddShadowInstanceID(unsigned int index) { m_ShadowInstances.push_back(index); }
	auto& GetShadowInstances() { return m_ShadowInstances; }
	auto& GetShadowInstanceSsbo() { return m_ShadowInstanceSsbo; }

	bool hasAnimation = false;
private:
	void GenerateBuffers();
	void Process(aiNode* node, const aiScene* scene);
	void ProcessGeometry(aiMesh* mesh, const aiScene* scene, unsigned int& count);
	std::string m_Path;
	std::string m_Directory;
private:
	unsigned int m_MeshCount;
	unsigned int m_IndexCount;
	unsigned int m_VertexCount;
	std::vector<Vertex> m_Vertices;
	std::vector<glm::uvec4> m_Instances;
	std::vector<std::string> m_MeshName;
	std::vector<unsigned int> m_Indices;
	std::vector<unsigned int> m_ShadowInstances;
	std::vector<MaterialComponent> m_Materials;
	std::unordered_map<std::string, unsigned int> m_FoundMaterials;
private: //GPU RESOURCES
	std::unique_ptr<VertexArrayGL> m_Vao;
	std::unique_ptr<BufferGL> m_Ibo;
	std::unique_ptr<BufferGL> m_Vbo;
	std::unique_ptr<ShaderStorageBufferGL> m_MaterialSsbo;
	std::unique_ptr<ShaderStorageBufferGL> m_InstanceSsbo;
	std::unique_ptr<ShaderStorageBufferGL> m_ShadowInstanceSsbo;
private: //OBB
	void GenerateObb();
	glm::vec3 m_ObbMax;
	glm::vec3 m_ObbMin;
	glm::vec3 m_ObbOrigin;
	glm::vec3 m_ObbExtents;
	std::array<glm::vec3, 8> m_Obb;
public: //LOD
	unsigned int m_LodLevels;
	std::vector<float> m_LodThresholds;
	std::vector<unsigned int> m_LodIndicesSize;
	std::vector<unsigned int> m_LodIndicesOffsets;
	std::vector<std::vector<unsigned int>> m_LodIndices;
};

