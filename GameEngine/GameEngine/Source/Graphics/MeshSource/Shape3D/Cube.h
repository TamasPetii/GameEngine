#pragma once
#include "../Shape.h"

class Cube : public Shape
{
public:
	Cube();
	Cube(GLfloat width, GLfloat height, GLfloat length);
	Cube(const json& data);

	json Serialize() const override;
	void DeSerialize(const json& data) override;
	Cube* Clone() const override;

	void GenerateVertices() override;
	void GenerateIndices() override;
	void GenerateNormals() override;
private:
	GLfloat m_Width; // X
	GLfloat m_Height; // Y
	GLfloat m_Length; // Z
};

