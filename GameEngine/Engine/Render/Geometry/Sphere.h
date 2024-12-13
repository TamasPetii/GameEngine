#pragma once
#include "EngineApi.h"
#include "Geometry.h"
#include <glm/gtc/constants.hpp>

class ENGINE_API Sphere : public Geometry
{
public:
	Sphere();
	Sphere(unsigned int count);
	virtual std::string GetName() { return "Sphere"; }
protected:
	virtual void GenerateVertices() override;
	virtual void GenerateIndices() override;
	virtual void GenerateSurfacePoints() override;
private:
	float m_Radius;
	unsigned int m_Count;
};

