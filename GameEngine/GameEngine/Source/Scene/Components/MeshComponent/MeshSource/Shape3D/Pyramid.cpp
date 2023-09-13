#include "Pyramid.h"

Pyramid::Pyramid()
{
	m_Name = "Pyramid";
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	GenerateShape(vertices, indices);
	UploadToGpu(vertices, indices);
}

Pyramid::Pyramid(const Pyramid& other)
{
	m_Name = other.m_Name;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	GenerateShape(vertices, indices);
	UploadToGpu(vertices, indices);
}

Pyramid* Pyramid::Clone() const
{
	return new Pyramid(*this);
}

void Pyramid::GenerateShape(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
	std::vector<glm::vec3> positions;
	positions.push_back(glm::vec3(0, 1, 0));
	positions.push_back(glm::vec3(-1, -1, 1));
	positions.push_back(glm::vec3(-1, -1, -1));
	positions.push_back(glm::vec3(1, -1, -1));
	positions.push_back(glm::vec3(1, -1, 1));

	for (unsigned int i = 1; i <= 4; i++)
	{
		unsigned int ind0 = 0;
		unsigned int ind1 = 1 + i % 4;
		unsigned int ind2 = i;

		vertices.push_back(Vertex(positions[ind0], glm::vec2(0.5, 1)));
		vertices.push_back(Vertex(positions[ind1], glm::vec2(0, 0)));
		vertices.push_back(Vertex(positions[ind2], glm::vec2(1, 0)));
	}

	//Bottom Face
	vertices.push_back(Vertex(positions[1], glm::vec2(0, 1)));
	vertices.push_back(Vertex(positions[2], glm::vec2(1, 1)));
	vertices.push_back(Vertex(positions[3], glm::vec2(1, 0)));
	vertices.push_back(Vertex(positions[4], glm::vec2(0, 0)));

	for (unsigned int i = 0; i < 12; i += 3)
	{
		unsigned int ind0 = i;
		unsigned int ind1 = i + 1;
		unsigned int ind2 = i + 2;

		indices.push_back(ind0);
		indices.push_back(ind1);
		indices.push_back(ind2);

		auto pos0 = std::make_tuple(vertices[ind0].position, vertices[ind1].position, vertices[ind2].position);
		auto tex0 = std::make_tuple(vertices[ind0].texture, vertices[ind1].texture, vertices[ind2].texture);
		auto tbn0 = Shape::GenerateTBN(pos0, tex0);

		vertices[ind0].tangent = std::get<0>(tbn0);
		vertices[ind1].tangent = std::get<0>(tbn0);
		vertices[ind2].tangent = std::get<0>(tbn0);

		vertices[ind0].bitangent = std::get<1>(tbn0);
		vertices[ind1].bitangent = std::get<1>(tbn0);
		vertices[ind2].bitangent = std::get<1>(tbn0);

		vertices[ind0].normal = std::get<2>(tbn0);
		vertices[ind1].normal = std::get<2>(tbn0);
		vertices[ind2].normal = std::get<2>(tbn0);
	}

	//Bottom Face
	indices.push_back(12 + 0);
	indices.push_back(12 + 1);
	indices.push_back(12 + 2);
	indices.push_back(12 + 0);
	indices.push_back(12 + 2);
	indices.push_back(12 + 3);

	auto normal = glm::vec3(0, -1, 0);
	auto TB = Shape::GenerateTB(normal);

	for (int j = 0; j < 4; j++)
	{
		vertices[12 + j].normal = normal;
		vertices[12 + j].tangent = TB.first;
		vertices[12 + j].bitangent = TB.second;
	}
}