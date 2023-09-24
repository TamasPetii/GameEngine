#pragma once
#include "../Mesh.h"

class ModelPart : public Mesh
{
public:
	ModelPart(const std::vector<Vertex>& vertex, const std::vector<GLuint>& indices, const Material& material, const Textures& texture);
	json Serialize() const override { return json(); }
	void DeSerialize(const json& data) override {};
	ModelPart* Clone() const override { return nullptr; }

	inline const auto& Get_Textures() const { return m_Textures; }
	inline const Material& Get_Material() const { return m_Material; }
private:
	Textures m_Textures;
	Material m_Material;
};

