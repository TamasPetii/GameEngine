#pragma once
#include "../Component.h"
#include "MeshSource/Mesh.h"

class MeshComponent : public Component
{
public:
	void Render();
	void AttachMesh(IMesh* mesh) { this->mMesh = mesh; }
private:
	IMesh* mMesh;
};

