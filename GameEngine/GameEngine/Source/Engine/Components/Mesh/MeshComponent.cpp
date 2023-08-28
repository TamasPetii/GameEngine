#include "MeshComponent.h"
#include "MeshSource/Shapes/Cylinder.h"

MeshComponent::MeshComponent()
{
}

MeshComponent::~MeshComponent()
{
	if(mMesh != nullptr)
		delete mMesh;
}

void MeshComponent::Render()
{
	mMesh->Render();
}