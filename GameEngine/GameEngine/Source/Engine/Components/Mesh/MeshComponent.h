#pragma once
#include "../Component.h"
#include "MeshSource/Mesh.h"
#include "MeshSource/Shapes/Sphere.h"
#include "MeshSource/Shapes/Torus.h"

struct MaterialProperties
{
	glm::vec3 ambient = glm::vec3(1);
	glm::vec3 diffuse = glm::vec3(1);;
	glm::vec3 specular = glm::vec3(1);;
};

struct TextureProperties
{
	ImageTexture* texture = nullptr;
	ImageTexture* normal = nullptr;
};

class MeshComponent : public Component
{
public:
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

