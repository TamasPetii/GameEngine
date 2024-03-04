#include "Pyramid.h"

Pyramid::Pyramid()
{
    GenerateSurfacePoints();
    GenerateVertices();
    GenerateIndices();
    GenerateTangents();
    GenerateBuffers();
    GenerateObb();
}

void Pyramid::GenerateSurfacePoints()
{
    m_Surfacepoints.reserve(5);

    float x = 1;
    float y = 1;
    float z = 1;

    m_Surfacepoints.push_back(glm::vec3(0, y, 0)); //Top
    m_Surfacepoints.push_back(glm::vec3(-x, -y, z)); //Bottom Front Left
    m_Surfacepoints.push_back(glm::vec3(-x, -y, -z)); //Bottom Back Left
    m_Surfacepoints.push_back(glm::vec3(x, -y, -z)); //Bottom Back Right
    m_Surfacepoints.push_back(glm::vec3(x, -y, z)); //Bottom Front Right
}

void Pyramid::GenerateVertices()
{
    m_Vertices.reserve(16);

    //Sides
    for (unsigned int i = 1; i <= 4; i++)
    {
        unsigned int ind0 = 0;
        unsigned int ind1 = 1 + i % 4;
        unsigned int ind2 = i;

        m_Vertices.push_back(Vertex(m_Surfacepoints[ind0], glm::vec2(0.5, 1)));
        m_Vertices.push_back(Vertex(m_Surfacepoints[ind1], glm::vec2(0, 0)));
        m_Vertices.push_back(Vertex(m_Surfacepoints[ind2], glm::vec2(1, 0)));
    }

    //Bottom Face
    m_Vertices.push_back(Vertex(m_Surfacepoints[1], glm::vec2(0, 1)));
    m_Vertices.push_back(Vertex(m_Surfacepoints[2], glm::vec2(1, 1)));
    m_Vertices.push_back(Vertex(m_Surfacepoints[3], glm::vec2(1, 0)));
    m_Vertices.push_back(Vertex(m_Surfacepoints[4], glm::vec2(0, 0)));
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
