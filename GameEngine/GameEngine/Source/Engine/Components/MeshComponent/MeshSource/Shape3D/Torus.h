#pragma once
#include "../Shape.h"

class Torus : public Shape
{
public:
	Torus();
private:
	GLuint m_Count;
	GLfloat m_RadiusPrimary;
	GLfloat m_RadiusSecondary;
};