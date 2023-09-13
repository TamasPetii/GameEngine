#pragma once
#include "../Shape.h"

class Pyramid : public Shape
{
public:
	Pyramid();
	Pyramid(const Pyramid& other);
	Pyramid* Clone() const override;
private:
	void GenerateShape(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
};
