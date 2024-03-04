#pragma once
#include "EngineApi.h"
#include "Geometry.h"
#include <glm/gtc/constants.hpp>

class ENGINE_API Torus : public Geometry
{
public:
	Torus();
protected:
	virtual void GenerateVertices() override;
	virtual void GenerateIndices() override;
	virtual void GenerateSurfacePoints() override;
private:
	GLuint m_Count{ 20 };
	GLfloat m_RadiusPrimary{0.8f};
	GLfloat m_RadiusSecondary{0.2f};
};

