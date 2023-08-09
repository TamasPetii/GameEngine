#pragma once
#include "Shape.h"

class Torus : public Shape
{
public:
	Torus();
	void CreateVertices() override;
	void CreateIndices() override;
private:
	unsigned int mColumn;
	unsigned int mRow;
	float mRadiusPrimary;
	float mRadiusSecondary;
};

