#pragma once
#include "../Shape.h"

class Grid : public Shape
{
public:
	Grid();
	Grid(GLfloat width, GLfloat height);
	Grid(const json& data);

	json Serialize() const override;
	void DeSerialize(const json& data) override;
	Grid* Clone() const override;

	void GenerateVertices() override;
	void GenerateIndices() override;
	void GenerateNormals() override;
private:
	GLfloat m_Width; // X
	GLfloat m_Length; // Z
};

