#pragma once
#include "../Mesh.h"

class Plane {};

template<>
class Shape<Plane> : public MeshBase
{
public:
	Shape();
	void LoadIndices() override;
	void LoadVertices() override;
};

