#pragma once
#include "../Shape.h"

class Capsule : public Shape
{
public:
	Capsule();
	Capsule(const Capsule& other);
	Capsule* Clone() const override;
private:
	enum class Side { TOP, BOTTOM };
	void GenerateShape(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
	void GenerateWall(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
	void GenerateHalfSphere(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, enum class Side side);
private:
	GLint m_Count;
	GLfloat m_Radius;
	GLfloat m_Height;
};

