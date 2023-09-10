#pragma once
#include "../Shape.h"

class Plane : public Shape
{
public:
	Plane();
	Plane(const Plane& other);
	Plane* Clone() const override;
private:
	void GenerateShape(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
};