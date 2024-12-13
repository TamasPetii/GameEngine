#pragma once
#include "EngineApi.h"
#include "Geometry.h"
#include <glm/gtc/constants.hpp>

class ENGINE_API Cylinder : public Geometry
{
public:
	Cylinder();
	virtual std::string GetName() { return "Cylinder"; }
protected:
	virtual void GenerateVertices() override;
	virtual void GenerateIndices() override;
	virtual void GenerateSurfacePoints() override;
private:
	float m_Height{2.f};
	float m_RadiusTop{1.f};
	float m_RadiusBottom{1.f};
	unsigned int m_Count{16};
};

