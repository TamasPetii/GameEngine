#pragma once
#include "../Mesh.h"

class Cube {};

template<>
class Shape<Cube> : public MeshBase
{
public:
	Shape();
	void LoadIndices() override;
	void LoadVertices() override;
	void CreateNormals();
};

