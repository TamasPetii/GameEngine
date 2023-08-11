#pragma once
#include "../Mesh.h"

class Sphere {};

template<>
class Shape<Sphere> : public MeshBase
{
public:
	Shape();
	void LoadIndices() override;
	void LoadVertices() override;
private:
	unsigned int mColumn;
	unsigned int mRow;
	float mRadius;
};