#include "Sphere.h"

Sphere::Sphere() : Sphere(20)
{
}

Sphere::Sphere(GLuint count)
{
    m_Count = count;
    GenerateSurfacePoints();
    GenerateVertices();
    GenerateIndices();
    GenerateTangents();
    GenerateBuffers();
    GenerateObb();
}

void Sphere::GenerateSurfacePoints()
{
    m_Surfacepoints.clear();
    m_Surfacepoints.reserve((m_Count + 1) * (m_Count + 1));

    for (unsigned int h = 0; h <= m_Count; h++)
    {
        for (unsigned int w = 0; w <= m_Count; w++)
        {
            float u = w / static_cast<float>(m_Count);
            float v = h / static_cast<float>(m_Count);
            float theta = 2 * glm::pi<float>() * u;
            float phi = glm::pi<float>() * (0.5f - v);

            float x = m_Radius * cosf(phi) * sinf(theta);
            float y = m_Radius * sinf(phi);
            float z = m_Radius * cosf(phi) * cosf(theta);

            m_Surfacepoints.push_back(glm::vec3(x, y, z));
        }
    }
}

void Sphere::GenerateVertices()
{
    m_Vertices.reserve((m_Count + 1) * (m_Count + 1));
    int index = 0;
    for (unsigned int h = 0; h <= m_Count; h++)
    {
        for (unsigned int w = 0; w <= m_Count; w++)
        {
            float u = w / static_cast<float>(m_Count);
            float v = h / static_cast<float>(m_Count);

            m_Vertices.push_back({ m_Surfacepoints[index], glm::normalize(m_Surfacepoints[index]), glm::vec2(u, 1 - v) });
            ++index;
        }
    }
}

void Sphere::GenerateIndices()
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
