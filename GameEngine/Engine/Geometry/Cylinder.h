#pragma once
#include "Geometry.h"
#include <glm/gtc/constants.hpp>

class Cylinder : public Geometry
{
public:
	Cylinder();
protected:
	virtual void GenerateVertices() override;
	virtual void GenerateIndices() override;
	virtual void GenerateSurfacePoints() override;
private:
	GLint m_Count{16};
	GLfloat m_Height{2.f};
	GLfloat m_RadiusTop{1.f};
	GLfloat m_RadiusBottom{1.f};
};

