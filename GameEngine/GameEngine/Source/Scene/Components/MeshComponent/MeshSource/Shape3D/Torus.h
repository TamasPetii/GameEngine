#pragma once
#include "../Shape.h"

class Torus : public Shape
{
public:
	Torus();
	Torus(const Torus& other);
	Torus* Clone() const override;
private:
	void GenerateShape(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
	GLuint m_Count;
	GLfloat m_RadiusPrimary;
	GLfloat m_RadiusSecondary;
};