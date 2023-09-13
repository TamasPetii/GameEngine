#include "Capsule.h"

Capsule::Capsule()
{
	m_Count = 25;
	m_Radius = 1.f;
	m_Height = 2.f;

	m_Name = "Capsule";
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	GenerateShape(vertices, indices);
	UploadToGpu(vertices, indices);
}

Capsule::Capsule(const Capsule& other)
{
	m_Count = other.m_Count;
	m_Radius = other.m_Radius;
	m_Height = other.m_Height;
	m_Name = other.m_Name;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	GenerateShape(vertices, indices);
	UploadToGpu(vertices, indices);
}

Capsule* Capsule::Clone() const
{
	return new Capsule(*this);
}

void Capsule::GenerateShape(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
	GenerateHalfSphere(vertices, indices, Side::TOP);
	GenerateHalfSphere(vertices, indices, Side::BOTTOM);
	GenerateWall(vertices, indices);
}

void Capsule::GenerateWall(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
	GLuint startVertexCount = vertices.size();

	for (int p = 0; p <= m_Count; p++)
	{
		float u = p / float(m_Count);
		float theta = u * (2 * M_PI);

		auto positionTop = glm::vec3(m_Radius * sinf(theta), m_Height / 2, m_Radius * cosf(theta));
		auto positionBottom = glm::vec3(m_Radius * sinf(theta), -m_Height / 2, m_Radius * cosf(theta));
		auto normal = glm::vec3(sinf(theta), 0, cosf(theta));
		auto TB = Shape::GenerateTB(normal);

		Vertex topVertex;
		topVertex.position = positionTop;
		topVertex.normal = glm::normalize(normal);
		topVertex.tangent = glm::normalize(TB.first);
		topVertex.bitangent = glm::normalize(TB.second);
		topVertex.texture = glm::vec2(1 - u, 0.75);
		vertices.push_back(topVertex);

		Vertex bottomVertex;
		bottomVertex.position = positionBottom;
		bottomVertex.normal = glm::normalize(normal);
		bottomVertex.tangent = glm::normalize(TB.first);
		bottomVertex.bitangent = glm::normalize(TB.second);
		bottomVertex.texture = glm::vec2(1 - u, 0.25);
		vertices.push_back(bottomVertex);

		int index = 0;
		int start = startVertexCount;
		for (int p = 0; p < m_Count; p++)
		{
			int c = start + index;
			indices.push_back(c);
			indices.push_back(c + 1);
			indices.push_back(c + 3);

			indices.push_back(c);
			indices.push_back(c + 3);
			indices.push_back(c + 2);

			index += 2;
		}
	}
}

void Capsule::GenerateHalfSphere(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, enum class Side side)
{
	GLuint startVertexCount = vertices.size();

	for (unsigned int h = 0; h <= m_Count; h++)
	{
		for (unsigned int w = 0; w <= m_Count; w++)
		{

			float u = w / static_cast<float>(m_Count);
			float v = h / static_cast<float>(m_Count);
			float theta = 2 * M_PI * u;
			float phi = M_PI * (0.5f - v);

			/*
			if (side == Side::TOP && phi < 0)
				continue;

			if (side == Side::BOTTOM && phi > 0)
				continue;
			*/

			float x = m_Radius * cosf(phi) * sinf(theta);
			float y = m_Radius * sinf(phi);
			float z = m_Radius * cosf(phi) * cosf(theta);

			auto normal = glm::vec3(x, y, z);
			auto TB = Shape::GenerateTB(normal);

			Vertex vertex;
			vertex.position = glm::vec3(x, y + (side == Side::TOP ? m_Height / 2 : -m_Height / 2), z);
			vertex.normal = glm::normalize(normal);
			vertex.tangent = glm::normalize(TB.first);
			vertex.bitangent = glm::normalize(TB.second);
			vertex.texture = glm::vec2(1 - u, (1 - v) * 0.5 + (side == Side::TOP ? 0.5 : 0));
			vertices.push_back(vertex);
		}
	}

	unsigned int start = startVertexCount;
	for (unsigned int h = 0; h < m_Count; h++)
	{
		for (unsigned int w = 0; w < m_Count; w++)
		{
			int c = start + w + h * (m_Count + 1);
			indices.push_back(c);
			indices.push_back(c + m_Count + 1);
			indices.push_back(c + m_Count + 2);
			indices.push_back(c);
			indices.push_back(c + m_Count + 2);
			indices.push_back(c + 1);
		}
	}
}