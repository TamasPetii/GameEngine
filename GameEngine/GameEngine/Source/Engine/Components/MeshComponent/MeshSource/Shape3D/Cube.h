#pragma once
#include "../Shape.h"

class Cube : public Shape
{
public:
	Cube();
	Cube(const Cube& other);
	Cube* Clone() const override;
private:
	void GenerateShape(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
};

