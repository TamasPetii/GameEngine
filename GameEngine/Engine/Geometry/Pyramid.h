#pragma once
#include "Geometry.h"

class Pyramid : public Geometry
{
public:
	Pyramid();
protected:
	virtual void GenerateVertices() override;
	virtual void GenerateIndices() override;
	virtual void GenerateSurfacePoints() override;
};

