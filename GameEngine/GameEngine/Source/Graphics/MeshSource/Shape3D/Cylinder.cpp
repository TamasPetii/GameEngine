#include "Cylinder.h"

Cylinder::Cylinder() :
	Cylinder(25, 2.f, 1.f, 1.f)
{
}

Cylinder::Cylinder(GLint count, GLfloat height, GLfloat radiusTop, GLfloat radiusBottom) :
	m_Count(count),
	m_Height(height),
	m_RadiusTop(radiusTop),
	m_RadiusBottom(radiusBottom)
{
	Shape::GenerateShape();
}

Cylinder::Cylinder(const json& data)
{
	DeSerialize(data);
	Shape::GenerateShape();
}

json Cylinder::Serialize() const
{
	json data;
	data["Cylinder"]["Count"] = m_Count;
	data["Cylinder"]["Height"] = m_Height;
	data["Cylinder"]["RadiusTop"] = m_RadiusTop;
	data["Cylinder"]["RadiusBottom"] = m_RadiusBottom;
	return data;
}

void Cylinder::DeSerialize(const json& data)
{
	m_Count = data["Count"];
	m_Height = data["Height"];
	m_RadiusTop = data["RadiusTop"];
	m_RadiusBottom = data["RadiusBottom"];
}

Cylinder* Cylinder::Clone() const
{
	auto data = Serialize();
	return new Cylinder(data["Cylinder"]);
}

void Cylinder::GenerateVertices()
{
	float y = m_Height / 2;
	std::vector<glm::vec3> topPositions;
	std::vector<glm::vec3> bottomPositions;

	for (int p = 0; p <= m_Count; p++)
	{
		float u = p / float(m_Count);
		float theta = u * (2 * M_PI);
		topPositions.push_back(glm::vec3(m_RadiusTop * sinf(theta), y, m_RadiusTop * cosf(theta)));
		bottomPositions.push_back(glm::vec3(m_RadiusBottom * sinf(theta), -y, m_RadiusBottom * cosf(theta)));
	}

	//Top Circle Vertices
	Vertex topMiddleVertex;
	topMiddleVertex.position = glm::vec3(0, y, 0);
	topMiddleVertex.texture = glm::vec2(0.5, 0.5);
	m_Vertices.push_back(topMiddleVertex);

	for (int p = 0; p <= m_Count; p++)
	{
		float u = p / float(m_Count);
		float theta = u * (2 * M_PI);

		Vertex vertex;
		vertex.position = topPositions[p];
		vertex.texture = glm::vec2((cosf(theta) + 1) * 0.5f, (sinf(theta) + 1) * 0.5f);
		m_Vertices.push_back(vertex);
	}

	//Bottom Circle Vertices
	Vertex bottomMiddleVertex;
	bottomMiddleVertex.position = glm::vec3(0, -y, 0);
	bottomMiddleVertex.texture = glm::vec2(0.5, 0.5);
	m_Vertices.push_back(bottomMiddleVertex);

	for (int p = 0; p <= m_Count; p++)
	{
		float u = p / float(m_Count);
		float theta = u * (2 * M_PI);

		Vertex vertex;
		vertex.position = bottomPositions[p];
		vertex.texture = glm::vec2((sinf(theta) + 1) * 0.5f, (cosf(theta) + 1) * 0.5f);
		m_Vertices.push_back(vertex);
	}

	//Wall Vertices
	for (int p = 0; p <= m_Count; p++)
	{
		float u = p / float(m_Count);
		float theta = u * (2 * M_PI);

		Vertex topVertex;
		topVertex.position = topPositions[p];
		topVertex.texture = glm::vec2(u, 1);
		m_Vertices.push_back(topVertex);

		Vertex bottomVertex;
		bottomVertex.position = bottomPositions[p];
		bottomVertex.texture = glm::vec2(u, 0);
		m_Vertices.push_back(bottomVertex);
	}
}

void Cylinder::GenerateIndices()
{
	//Top Circle Indices
	int start = 0;
	for (int p = 0; p < m_Count; p++)
	{
		m_Indices.push_back(start);
		m_Indices.push_back(start + p + 1);
		m_Indices.push_back(start + p + 2);
	}

	//Bottom Circle Indices
	start = 1 + m_Count + 1;
	for (int p = 0; p < m_Count; p++)
	{
		m_Indices.push_back(start);
		m_Indices.push_back(start + p + 2);
		m_Indices.push_back(start + p + 1);
	}

	//Wall Indices
	int index = 0;
	start = 2 * (1 + m_Count + 1);
	for (int p = 0; p < m_Count; p++)
	{
		int c = start + index;
		m_Indices.push_back(c);
		m_Indices.push_back(c + 1);
		m_Indices.push_back(c + 3);

		m_Indices.push_back(c);
		m_Indices.push_back(c + 3);
		m_Indices.push_back(c + 2);

		index += 2;
	}
}

void Cylinder::GenerateNormals()
{
	//Top Circle Indices
	int start = 0;
	for (int p = 0; p < m_Count; p++)
	{
		Mesh::GenerateTbn(m_Vertices[start], m_Vertices[start + p + 1], m_Vertices[start + p + 2], false);
	}

	//Bottom Circle Indices
	start = 1 + m_Count + 1;
	for (int p = 0; p < m_Count; p++)
	{
		Mesh::GenerateTbn(m_Vertices[start], m_Vertices[start + p + 1], m_Vertices[start + p + 2], false);
	}

	//Wall Indices
	int index = 0;
	start = 2 * (1 + m_Count + 1);
	for (int p = 0; p < m_Count; p++)
	{
		int c = start + index;
		Mesh::GenerateTbn(m_Vertices[c], m_Vertices[c + 1], m_Vertices[c + 3], false);
		Mesh::GenerateTbn(m_Vertices[c], m_Vertices[c + 3], m_Vertices[c + 2], false);
		index += 2;
	}
}