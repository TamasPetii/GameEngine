#pragma once
#include "../Mesh.h"

class Torus {};

template<>
class Shape<Torus> : public MeshBase
{
public:
	Shape();
	void LoadIndices() override;
	void LoadVertices() override;
private:
	unsigned int mColumn;
	unsigned int mRow;
	float mRadiusPrimary;
	float mRadiusSecondary;
};