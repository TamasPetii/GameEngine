#pragma once
#include "../Mesh.h"
#include <fstream>
#include <sstream>
#include <string>
#include <GLM/gtc/matrix_transform.hpp>

#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <ASSIMP/postprocess.h>

#include "ModelPart.h"

class Model : public IMesh
{
public:
	static Model* LoadModel(const std::string& path);
	 void Render() override {};
	 void Render(IFrameBufferObject* frameBuffer, Program* shaderProgram, const glm::mat4& transform);
protected:
	 ModelPart* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	 void ProcessNode(aiNode* node, const aiScene* scene);

private:
	std::string mPath;
	std::vector<ModelPart*> mParts;
	static std::unordered_map<std::string, Model*> m_LoadedModels;
};

