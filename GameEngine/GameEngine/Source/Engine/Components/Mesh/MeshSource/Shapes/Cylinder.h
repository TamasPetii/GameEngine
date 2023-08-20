#pragma once
#include "../Mesh.h"

class Cylinder {};

struct CylinderLayout
{
	int mCount;
	float mHeight;
	float mRadiusTop;
	float mRadiusBottom;
};

template<>
class Shape<Cylinder> : public MeshBase
{
public:
	Shape();
	void LoadIndices() override;
	void LoadVertices() override;
	void HardNormals();
	void ShadeNormals();
	void RefreshVertices();
	void RefreshShape();
	CylinderLayout& GetLayout() { return mLayout; }
private:
	CylinderLayout mLayout;
};
