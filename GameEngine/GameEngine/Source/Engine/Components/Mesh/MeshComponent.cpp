#include "MeshComponent.h"

MeshComponent::MeshComponent()
{
	mColor = glm::vec3(1);
	mTexture = nullptr;
}

MeshComponent::~MeshComponent()
{
	delete mMesh;
}

void MeshComponent::Render()
{
	mMesh->Render();
}
