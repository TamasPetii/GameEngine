#pragma once
#include "../Component.h"
#include "MeshSource/Mesh.h"
#include "MeshSource/Shapes/Sphere.h"
#include "MeshSource/Shapes/Torus.h"

class MeshComponent : public Component
{
public:
	MeshComponent();
	~MeshComponent();
	void Render();
	void ChangeShade();
	void AttachMesh(IMesh* mesh) { this->mMesh = mesh; }
	IMesh* GetMesh() { return mMesh; }
	inline glm::vec3& GetColor() { return mColor; }
	inline Texture2D* GetTexture() { return mTexture; }
	inline void SetTexture(Texture2D* texture) { mTexture = texture; }
	inline bool& GetShadeSmooth() { return mShadeSmooth; }
private:
	IMesh* mMesh;
	bool mShadeSmooth;
	glm::vec3 mColor;
	Texture2D* mTexture;
};

