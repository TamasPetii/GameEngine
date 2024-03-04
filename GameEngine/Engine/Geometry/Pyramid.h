#pragma once
#include "EngineApi.h"
#include "Geometry.h"

class ENGINE_API Pyramid : public Geometry
{
public:
	Pyramid();
protected:
	virtual void GenerateVertices() override;
	virtual void GenerateIndices() override;
	virtual void GenerateSurfacePoints() override;
};

