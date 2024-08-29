#include "Torus.h"

Torus::Torus()
{
    GenerateSurfacePoints();
    GenerateVertices();
    GenerateIndices();
    GenerateTangents();
    GenerateBuffers();
    GenerateObb();
}

void Torus::GenerateSurfacePoints()
{
    m_Surfacepoints.reserve((m_Count + 1) * (m_Count + 1));

    for (unsigned int h = 0; h <= m_Count; h++)
    {
        for (unsigned int w = 0; w <= m_Count; w++)
        {
            float u = w / static_cast<float>(m_Count);
            float v = h / static_cast<float>(m_Count);
            float theta = 2 * glm::pi<float>() * u;
            float phi = 2 * glm::pi<float>() * v;

            float x = (m_RadiusPrimary + m_RadiusSecondary * cosf(phi)) * cosf(theta);
            float y = m_RadiusSecondary * sinf(phi);
            float z = (m_RadiusPrimary + m_RadiusSecondary * cosf(phi)) * sinf(theta);

            m_Surfacepoints.push_back(glm::vec3(x, y, z));
        }
    }
}

void Torus::GenerateVertices()
{
    int index = 0;

    for (unsigned int h = 0; h <= m_Count; h++)
    {
        for (unsigned int w = 0; w <= m_Count; w++)
        {
            float u = w / static_cast<float>(m_Count);
            float v = h / static_cast<float>(m_Count);

            float theta = 2 * glm::pi<float>() * u;
            float phi = 2 * glm::pi<float>() * v;

            float x = cosf(phi) * cosf(theta);
            float y = sinf(phi);
            float z = cosf(phi) * sinf(theta);

            Vertex vertex;
            vertex.position = m_Surfacepoints[index];
            vertex.normal = glm::normalize(glm::vec3(x, y, z));
            vertex.texcoord = glm::vec2(u, 1 - v);
            m_Vertices.push_back(vertex);

            ++index;
        }
    }
}

void Torus::GenerateIndices()
{
    for (unsigned int h = 0; h < m_Count; h++)
    {
        for (unsigned int w = 0; w < m_Count; w++)
        {
            int c = w + h * (m_Count + 1);
            m_Indices.push_back(c);
            m_Indices.push_back(c + m_Count + 1);
            m_Indices.push_back(c + m_Count + 2);
            m_Indices.push_back(c);
            m_Indices.push_back(c + m_Count + 2);
            m_Indices.push_back(c + 1);
        }
    }
}

