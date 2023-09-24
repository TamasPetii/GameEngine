#pragma once
#include "../Shape.h"

class Plane : public Shape
{
public:
	Plane();
	Plane(GLfloat width, GLfloat length);
	Plane(const json& data);

	json Serialize() const override;
	void DeSerialize(const json& data) override;
	Plane* Clone() const override;

	void GenerateVertices() override;
	void GenerateIndices() override;
	void GenerateNormals() override;
private:
	GLfloat m_Width; // X
	GLfloat m_Length; // Z
};