#include "Pyramid.h"

Pyramid::Pyramid() :
	Pyramid(2.f, 2.f, 2.f)
{
}

Pyramid::Pyramid(GLfloat width, GLfloat height, GLfloat length) :
	m_Width(width),
	m_Height(height),
	m_Length(length)
{
	Shape::GenerateShape();
}

Pyramid::Pyramid(const json& data)
{
	DeSerialize(data);
	Shape::GenerateShape();
}

json Pyramid::Serialize() const
{
	json data;
	data["Pyramid"]["Width"] = m_Width;
	data["Pyramid"]["Height"] = m_Height;
	data["Pyramid"]["Length"] = m_Length;
	return data;
}

void Pyramid::DeSerialize(const json& data)
{
	m_Width = data["Width"];
	m_Height = data["Height"];
	m_Length = data["Length"];
}

Pyramid* Pyramid::Clone() const
{
	auto data = Serialize();
	return new Pyramid(data["Pyramid"]);
}

void Pyramid::GenerateVertices()
{
	float x = m_Width / 2;
	float y = m_Height / 2;
	float z = m_Length / 2;

	std::vector<glm::vec3> positions;
	positions.push_back(glm::vec3(0, y, 0));
	positions.push_back(glm::vec3(-x, -y, z));
	positions.push_back(glm::vec3(-x, -y, -z));
	positions.push_back(glm::vec3(x, -y, -z));
	positions.push_back(glm::vec3(x, -y, z));

	//Sides
	for (unsigned int i = 1; i <= 4; i++)
	{
		unsigned int ind0 = 0;
		unsigned int ind1 = 1 + i % 4;
		unsigned int ind2 = i;

		m_Vertices.push_back(Vertex(positions[ind0], glm::vec2(0.5, 1)));
		m_Vertices.push_back(Vertex(positions[ind1], glm::vec2(0, 0)));
		m_Vertices.push_back(Vertex(positions[ind2], glm::vec2(1, 0)));
	}

	//Bottom Face
	m_Vertices.push_back(Vertex(positions[1], glm::vec2(0, 1)));
	m_Vertices.push_back(Vertex(positions[2], glm::vec2(1, 1)));
	m_Vertices.push_back(Vertex(positions[3], glm::vec2(1, 0)));
	m_Vertices.push_back(Vertex(positions[4], glm::vec2(0, 0)));
}

void Pyramid::GenerateIndices()
{
	//Sides
	for (unsigned int i = 0; i < 12; i += 3)
	{
		m_Indices.push_back(i);
		m_Indices.push_back(i + 1);
		m_Indices.push_back(i + 2);
	}

	//Bottom Face
	m_Indices.push_back(12 + 0);
	m_Indices.push_back(12 + 1);
	m_Indices.push_back(12 + 2);
	m_Indices.push_back(12 + 0);
	m_Indices.push_back(12 + 2);
	m_Indices.push_back(12 + 3);
}

void Pyramid::GenerateNormals()
{
	//Sides
	for (unsigned int i = 0; i < 12; i += 3)
	{
		Mesh::GenerateTbn(m_Vertices[i], m_Vertices[i + 1], m_Vertices[i + 2], false);
	}

	Mesh::GenerateTbn(m_Vertices[12 + 0], m_Vertices[12 + 1], m_Vertices[12 + 2], false);
	Mesh::GenerateTbn(m_Vertices[12 + 0], m_Vertices[12 + 2], m_Vertices[12 + 3], false);
}