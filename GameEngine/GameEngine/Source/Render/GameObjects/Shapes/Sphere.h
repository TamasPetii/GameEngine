#pragma once
#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere();
	void CreateVertices() override;
	void CreateIndices() override;
private:
	unsigned int mColumn;
	unsigned int mRow;
	float mRadius;
};


