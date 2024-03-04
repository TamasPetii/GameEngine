#include "Cone.h"

Cone::Cone() : Cone(20)
{
}

Cone::Cone(GLuint count)
{
    m_Count = count;
    GenerateSurfacePoints();
    GenerateVertices();
    GenerateIndices();
    //GenerateTangents();
    GenerateBuffers();
    GenerateObb();
}

void Cone::GenerateSurfacePoints()
{
    m_Surfacepoints.clear();
    m_Surfacepoints.reserve(2 + m_Count + 1);

    m_Surfacepoints.push_back(glm::vec3(0, 1, 0));
    m_Surfacepoints.push_back(glm::vec3(0, -1, 0));

    for (int i = 0; i <= m_Count; ++i)
    {
        float u = i / static_cast<float>(m_Count);
        float theta = 2 * glm::pi<float>() * u;

        m_Surfacepoints.push_back(glm::vec3(sinf(theta), -1, cosf(theta)));
    }
}

void Cone::GenerateVertices()
{
    m_Vertices.reserve(2 + m_Count + 1);
    m_Vertices.push_back(Vertex(m_Surfacepoints[0], glm::vec3(0, 1, 0), glm::vec2(0.5, 1)));
    m_Vertices.push_back(Vertex(m_Surfacepoints[1], glm::vec3(0, -1, 0), glm::vec2(0.5, 0.5)));
    
    //Top
    for (int i = 0; i <= m_Count; ++i)
    {
        float u = i / static_cast<float>(m_Count);
        float theta = 2 * glm::pi<float>() * u;

        m_Vertices.push_back(Vertex(m_Surfacepoints[2 + i], glm::vec3(sinf(theta), 0, cosf(theta)), glm::vec2(u, 0)));
    }

    //Bottom
    for (int i = 0; i <= m_Count; ++i)
    {
        float u = i / static_cast<float>(m_Count);
        float theta = 2 * glm::pi<float>() * u;

        m_Vertices.push_back(Vertex(m_Surfacepoints[2 + i], glm::vec3(sinf(theta), -1, cosf(theta)), glm::vec2((sinf(theta) + 1) * 0.5f, (cosf(theta) + 1) * 0.5f)));
    }
}

void Cone::GenerateIndices()
{
    for (unsigned int i = 0; i < m_Count; i++)
    {
        m_Indices.push_back(0);
        m_Indices.push_back(2 + i);
        m_Indices.push_back(2 + i + 1);
        m_Indices.push_back(1);
        m_Indices.push_back(2 + i + 1 + m_Count + 1);
        m_Indices.push_back(2 + i + m_Count + 1);
    }
}
