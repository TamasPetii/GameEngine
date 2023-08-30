#pragma once
#include "../Component.h"
#include "MeshSource/Mesh.h"

struct MaterialProperties
{
	glm::vec3 ambient = glm::vec3(1);
	glm::vec3 diffuse = glm::vec3(1);;
	glm::vec3 specular = glm::vec3(1);;
};

struct TextureProperties
{
	float scale = 1;
	ImageTexture* texture = nullptr;
	ImageTexture* normal = nullptr;
	ImageTexture* height = nullptr;
};

class MeshComponent : public Component
{
public:
	json SaveToJson() const override { return json(); }
	void LoadFromJson(const json& object) override {}

	MeshComponent();
	~MeshComponent();
	void Render();
	void AttachMesh(IMesh* mesh) { this->mMesh = mesh; }
	inline auto& GetMesh() { return mMesh; }
	inline auto& Get_Material() { return mMaterial; }
	inline auto& Get_Textures() { return mTextures; }
private:
	IMesh* mMesh;
	bool mShadeSmooth;

	MaterialProperties mMaterial;
	TextureProperties mTextures;
};

