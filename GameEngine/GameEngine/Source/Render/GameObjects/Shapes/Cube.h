#pragma once
#include "Shape.h"

class Cube : public Shape
{
public:
	Cube();
	void CreateVertices() override;
	void CreateIndices() override;
};

