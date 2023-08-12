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

void MeshComponent::ChangeShade()
{
	Shape<Sphere>* sphere = dynamic_cast<Shape<Sphere>*>(mMesh);
	if (sphere)
	{
		if (mShadeSmooth)
			sphere->ShadeNormals();
		else
			sphere->HardNormals();
		sphere->RefreshVertices();
	}

	Shape<Torus>* torus = dynamic_cast<Shape<Torus>*>(mMesh);
	if (torus)
	{
		if (mShadeSmooth)
			torus->ShadeNormals();
		else
			torus->HardNormals();
		torus->RefreshVertices();
	}
}