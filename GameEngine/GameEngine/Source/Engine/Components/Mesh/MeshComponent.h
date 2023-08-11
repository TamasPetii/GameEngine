#pragma once
#include "../Component.h"
#include "MeshSource/Mesh.h"

class MeshComponent : public Component
{
public:
	MeshComponent();
	~MeshComponent(); //TODO: Delete mesh
	void Render();
	void AttachMesh(IMesh* mesh) { this->mMesh = mesh; }
	inline glm::vec3& GetColor() { return mColor; }
	inline Texture2D* GetTexture() { return mTexture; }
	inline void SetTexture(Texture2D* texture) { mTexture = texture; }
private:
	IMesh* mMesh;
	glm::vec3 mColor;
	Texture2D* mTexture;
};

