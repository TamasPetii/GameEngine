#pragma once
#include "../Shape.h"

class Pyramid : public Shape
{
public:
	Pyramid();
	Pyramid(GLfloat width, GLfloat height, GLfloat length);
	Pyramid(const json& data);

	json Serialize() const override;
	void DeSerialize(const json& data) override;
	Pyramid* Clone() const override;

	void GenerateVertices() override;
	void GenerateIndices() override;
	void GenerateNormals() override;
private:
	GLfloat m_Width; // X
	GLfloat m_Height; // Y
	GLfloat m_Length; // Z
};
