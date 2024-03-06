#pragma once
#include "EngineApi.h"
#include <queue>
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include "Registry/Component/MaterialComponent.h"
#include "Render/OpenGL/Vertex.h"
#include "Render/OpenGL/BufferGL.h"
#include "Render/OpenGL/VertexArrayGL.h"
#include "Render/OpenGL/ShaderStorageBufferGL.h"
#include "Model/AssimpConverter.h"

struct VertexBoneData
{
	glm::vec4 weights;
	glm::ivec4 indices;
};

struct BoneInfo
{
	GLuint index;
	glm::mat4 offset;
};

class ENGINE_API Animation
{
public:
	Animation();
	void Load(const std::string& path);
private:
	std::vector<VertexBoneData> m_VertexBoneData;
};

