#pragma once
#include "../Shape.h"

class Cylinder : public Shape
{
public:
	Cylinder();
	Cylinder(GLint count, GLfloat height, GLfloat radiusTop, GLfloat radiusBottom);
	Cylinder(const json& data);

	json Serialize() const override;
	void DeSerialize(const json& data) override;
	Cylinder* Clone() const override;

	void GenerateVertices() override;
	void GenerateIndices() override;
	void GenerateNormals() override;
private:
	GLint m_Count;
	GLfloat m_Height;
	GLfloat m_RadiusTop;
	GLfloat m_RadiusBottom;
};