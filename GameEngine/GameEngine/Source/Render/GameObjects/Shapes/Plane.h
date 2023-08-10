#pragma once
#include "Shape.h"

class Plane : public Shape
{
public:
	Plane();
	void CreateVertices() override;
	void CreateIndices() override;
};

