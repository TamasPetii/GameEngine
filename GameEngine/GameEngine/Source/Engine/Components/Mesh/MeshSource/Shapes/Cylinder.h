#pragma once
#include "../Mesh.h"

class Cylinder {};

template<>
class Shape<Cylinder> : public MeshBase
{
public:
	Shape();
	void LoadIndices() override;
	void LoadVertices() override;
	void CreateNormals();
private:
	int mPoints;
	float mHeight;
	float mRadiusTop;
	float mRadiusBottom;
};