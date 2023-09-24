#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <GLM/gtc/matrix_transform.hpp>
#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <ASSIMP/postprocess.h>
#include "ModelPart.h"
#include "../../../IRenderable.h"
#include "../../../Graphics/Texture/ImageTexture.h"

namespace Assimp
{
	inline glm::vec3 ConvertAssimpToGlm(const aiVector3D& vector)
	{
		return glm::vec3(vector.x, vector.y, vector.z);
	}

	inline glm::vec3 ConvertAssimpToGlm(const aiColor3D& vector)
	{
		return glm::vec3(vector.r, vector.g, vector.b);
	}
}

class Model : public IRenderable, public ISerializable, public ICloneable
{
public:
	static std::unordered_map<std::string, Model*> LOADED_MODELS;
	static Model* LoadModel(const std::string& path);

	json Serialize() const override;
	void DeSerialize(const json& data) override;
	Model* Clone() const override;

	inline const auto& Get_Path() const { return m_Path; }
	inline const auto& Get_Parts() const { return m_Parts; }
private:
	ModelPart* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void ProcessNode(aiNode* node, const aiScene* scene);
private:
	std::string m_Path;
	std::vector<ModelPart*> m_Parts;
};

inline std::unordered_map<std::string, Model*> Model::LOADED_MODELS;