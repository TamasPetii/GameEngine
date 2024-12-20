#pragma once
#include "EngineApi.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>
#include <assimp/types.h>
#include <assimp/scene.h>
#include "Bone.h"

struct ENGINE_API NodeData
{
	std::string name;
	glm::mat4 transform;
	std::vector<std::shared_ptr<NodeData>> children;
};

struct ENGINE_API VertexBoneData
{
	VertexBoneData();

	glm::vec4 weights;
	glm::ivec4 indices;
};

struct ENGINE_API BoneInfo
{
	BoneInfo();

	unsigned int index;
	glm::mat4 offset;
};

class ShaderStorageBufferGL;

class ENGINE_API Animation
{
public:
	Animation();
	virtual ~Animation();
	bool Load(const std::string& path);

	const auto& GetRoot() const { return root; }
	const auto& GetDuration() const { return m_Duration; }
	const auto& GetMeshCount() const { return m_MeshCount; }
	const auto& GetBoneCount() const { return m_BoneCount; }
	const auto& GetVertexCount() const { return m_VertexCount; }
	const auto& GetTicksPerSecond() const { return m_TicksPerSecond; }
	const auto& GetBoneInfos() const { return m_BoneInfos; }
	const auto& GetVertexBoneSsbo() const { return m_VertexBoneInfoSsbo; }
	const auto& GetPath() const { return m_Path; }
	auto& GetBones() { return m_Bones; }
	auto& RefRoot() { return root; }
	auto& RefDuration() { return m_Duration; }
	auto& RefMeshCount() { return m_MeshCount; }
	auto& RefBoneCount() { return m_BoneCount; }
	auto& RefVertexCount() { return m_VertexCount; }
	auto& RefTicksPerSecond() { return m_TicksPerSecond; }
	auto& RefBones() { return m_Bones; }
	auto& RefBoneInfos() { return m_BoneInfos; }
	auto& RefVertexBoneSsbo() { return m_VertexBoneInfoSsbo; }
private:
	void PreProcess(aiNode* node, const aiScene* scene);
	void Process(aiNode* node, const aiScene* scene);
	void ProcessBone(aiMesh* mesh, const aiScene* scene, unsigned int& count);
	void ProcessHierarchy(aiNode* node);
	void ProcessMissingBones(aiAnimation* animation);
private:
	std::string m_Path;
	int m_MeshCount;
	int m_BoneCount;
	int m_VertexCount;
	float m_Duration;
	float m_TicksPerSecond;
	std::vector<Bone> m_Bones;
	std::shared_ptr<NodeData> root;
	std::vector<VertexBoneData> m_VertexBoneData;
	std::unordered_map<std::string, BoneInfo> m_BoneInfos;
	std::shared_ptr<ShaderStorageBufferGL> m_VertexBoneInfoSsbo;
};

