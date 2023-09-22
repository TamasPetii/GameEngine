#pragma once
#include "../Shape.h"

class Grid : public Shape
{
public:
	Grid();
	Grid(const std::vector<std::vector<float>>& heightMap);
	Grid(const Grid& other);
	Grid* Clone() const override;
private:
	unsigned int m_Width;
	unsigned int m_Height;
	void GenerateShape(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
	void GenerateShape(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, const std::vector<std::vector<float>>& heightMap);
};