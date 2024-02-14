#pragma once
#include "Geometry.h"
#include <glm/gtc/constants.hpp>

class Cone : public Geometry
{
public:
	Cone();
	Cone(GLuint count);
protected:
	virtual void GenerateVertices() override;
	virtual void GenerateIndices() override;
	virtual void GenerateSurfacePoints() override;
private:
	GLuint m_Count{ 20 };
	GLfloat m_Radius{ 1.f };
};