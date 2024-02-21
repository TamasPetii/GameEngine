#include "Cylinder.h"

Cylinder::Cylinder()
{
    GenerateSurfacePoints();
    GenerateVertices();
    GenerateIndices();
    GenerateTangents();
    GenerateBuffers();
	GenerateObb();
}

void Cylinder::GenerateSurfacePoints()
{
    m_Surfacepoints.reserve(2 * (m_Count + 1) + 2);
    float y = m_Height / 2;

    m_Surfacepoints.push_back(glm::vec3(0, y, 0));
    m_Surfacepoints.push_back(glm::vec3(0, -y, 0));

    for (int p = 0; p <= m_Count; p++)
    {
        float u = p / float(m_Count);
        float theta = u * (2 * glm::pi<float>());

        m_Surfacepoints.push_back(glm::vec3(m_RadiusTop * sinf(theta), y, m_RadiusTop * cosf(theta)));
        m_Surfacepoints.push_back(glm::vec3(m_RadiusBottom * sinf(theta), -y, m_RadiusBottom * cosf(theta)));
    }
}

void Cylinder::GenerateVertices()
{
	//Top Circle Vertices
	Vertex topMiddleVertex;
	topMiddleVertex.position = m_Surfacepoints[0];
	topMiddleVertex.texcoord = glm::vec2(0.5, 0.5);
	m_Vertices.push_back(topMiddleVertex);

	for (int p = 0; p <= m_Count; p++)
	{
		float u = p / float(m_Count);
		float theta = u * (2 * glm::pi<float>());

		Vertex vertex;
		vertex.position = m_Surfacepoints[2 + 2 * p];
		vertex.texcoord = glm::vec2((cosf(theta) + 1) * 0.5f, (sinf(theta) + 1) * 0.5f);
		m_Vertices.push_back(vertex);
	}

	//Bottom Circle Vertices
	Vertex bottomMiddleVertex;
	bottomMiddleVertex.position = m_Surfacepoints[1];
	bottomMiddleVertex.texcoord = glm::vec2(0.5, 0.5);
	m_Vertices.push_back(bottomMiddleVertex);

	for (int p = 0; p <= m_Count; p++)
	{
		float u = p / float(m_Count);
		float theta = u * (2 * glm::pi<float>());

		Vertex vertex;
		vertex.position = m_Surfacepoints[3 + 2 * p];
		vertex.texcoord = glm::vec2((sinf(theta) + 1) * 0.5f, (cosf(theta) + 1) * 0.5f);
		m_Vertices.push_back(vertex);
	}

	//Wall Vertices
	for (int p = 0; p <= m_Count; p++)
	{
		float u = p / float(m_Count);
		float theta = u * (2 * glm::pi<float>());

		Vertex topVertex;
		topVertex.position = m_Surfacepoints[2 + 2 * p];
		topVertex.normal   = m_Surfacepoints[2 + 2 * p];
		topVertex.texcoord = glm::vec2(u, 1);
		m_Vertices.push_back(topVertex);

		Vertex bottomVertex;
		bottomVertex.position = m_Surfacepoints[3 + 2 * p];
		bottomVertex.normal   = m_Surfacepoints[3 + 2 * p];
		bottomVertex.texcoord = glm::vec2(u, 0);
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

