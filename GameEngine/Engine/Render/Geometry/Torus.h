#pragma once
#include "EngineApi.h"
#include "Geometry.h"
#include <glm/gtc/constants.hpp>

class ENGINE_API Torus : public Geometry
{
public:
	Torus();
	virtual std::string GetName() { return "Torus"; }
protected:
	virtual void GenerateVertices() override;
	virtual void GenerateIndices() override;
	virtual void GenerateSurfacePoints() override;
private:
	unsigned int m_Count;
	float m_RadiusPrimary;
	float m_RadiusSecondary;
};

