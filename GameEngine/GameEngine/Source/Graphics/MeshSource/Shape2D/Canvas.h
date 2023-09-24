#pragma once
#include "../Shape.h"

class Canvas : public Shape
{
public:
	Canvas();
	Canvas(GLfloat width, GLfloat height);
	Canvas(const json& data);

	json Serialize() const override;
	void DeSerialize(const json& data) override;
	Canvas* Clone() const override;

	void GenerateVertices() override;
	void GenerateIndices() override;
	void GenerateNormals() override;
private:
	GLfloat m_Width; // X
	GLfloat m_Height; // Y
};
