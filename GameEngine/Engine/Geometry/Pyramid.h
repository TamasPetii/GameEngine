#pragma once
#include "EngineApi.h"
#include "Geometry.h"

class ENGINE_API Pyramid : public Geometry
{
public:
	Pyramid();
	virtual std::string GetName() { return "Pyramid"; }
protected:
	virtual void GenerateVertices() override;
	virtual void GenerateIndices() override;
	virtual void GenerateSurfacePoints() override;
};

