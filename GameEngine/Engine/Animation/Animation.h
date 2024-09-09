#pragma once
#include "EngineApi.h"
#include <queue>
#include <stack>
#include <iostream>
#include <unordered_map>

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include "Render/OpenGL/Vertex.h"
#include "Render/OpenGL/BufferGL.h"
#include "Render/OpenGL/VertexArrayGL.h"
#include "Render/OpenGL/ShaderStorageBufferGL.h"
#include "Model/AssimpConverter.h"
#include "Bone.h"

struct ENGINE_API NodeData
{
	std::string name;
	glm::mat4 transform;
	std::vector<std::shared_ptr<NodeData>> children;
};

struct ENGINE_API VertexBoneData
{
	VertexBoneData() : 
		weights(0.f, 0.f, 0.f, 0.f),
		indices(-1, -1, -1, -1) {}

	glm::vec4 weights;
	glm::ivec4 indices;
};

struct ENGINE_API BoneInfo
{
	GLuint index;
	glm::mat4 offset;
};

class ENGINE_API Animation
{
public:
	Animation();
	bool Load(const std::string& path);

	const auto& GetRoot() const { return root; }
	const auto& GetDuration() const { return m_Duration; }
	const auto& GetMeshCount() const { return m_MeshCount; }
	const auto& GetBoneCount() const { return m_BoneCount; }
	const auto& GetVertexCount() const { return m_VertexCount; }
	const auto& GetTicksPerSecond() const { return m_TicksPerSecond; }
	const auto& GetBones() const { return m_Bones; }
	const auto& GetBoneInfos() const { return m_BoneInfos; }
	const auto& GetVertexBoneSsbo() const { return m_VertexBoneInfoSsbo; }
	const auto& GetPath() const { return m_Path; }
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

