#include "Cube.h"

Cube::Cube()
{
    GenerateSurfacePoints();
    GenerateVertices();
    GenerateIndices();
    GenerateTangents();
    GenerateBuffers();
    GenerateObb();
}

void Cube::GenerateSurfacePoints()
{
    m_Surfacepoints.clear();
    m_Surfacepoints.reserve(8);

    float x = 1;
    float y = 1;
    float z = 1;

    m_Surfacepoints.push_back(glm::vec3(-x, y, z)); //Front Top Left
    m_Surfacepoints.push_back(glm::vec3(-x, -y, z)); //Front Bottom Left
    m_Surfacepoints.push_back(glm::vec3(x, -y, z)); //Front Bottom Right
    m_Surfacepoints.push_back(glm::vec3(x, y, z)); //Front Top Right

    m_Surfacepoints.push_back(glm::vec3(-x, y, -z)); //Back Top Left
    m_Surfacepoints.push_back(glm::vec3(-x, -y, -z)); //Back Bottom Left
    m_Surfacepoints.push_back(glm::vec3(x, -y, -z)); //Back Bottom Right
    m_Surfacepoints.push_back(glm::vec3(x, y, -z)); //Back Top Right
}

void Cube::GenerateVertices()
{
    m_Vertices.reserve(24);

    //Front
    m_Vertices.push_back({ m_Surfacepoints[FRONT_TOP_LEFT],     glm::vec3(0, 0, 1), glm::vec2(0, 1) });
    m_Vertices.push_back({ m_Surfacepoints[FRONT_BOTTOM_LEFT],  glm::vec3(0, 0, 1), glm::vec2(0, 0) });
    m_Vertices.push_back({ m_Surfacepoints[FRONT_BOTTOM_RIGHT], glm::vec3(0, 0, 1), glm::vec2(1, 0) });
    m_Vertices.push_back({ m_Surfacepoints[FRONT_TOP_RIGHT],    glm::vec3(0, 0, 1), glm::vec2(1, 1) });
    //Back
    m_Vertices.push_back({ m_Surfacepoints[BACK_TOP_RIGHT],     glm::vec3(0, 0, -1), glm::vec2(0, 1) });
    m_Vertices.push_back({ m_Surfacepoints[BACK_BOTTOM_RIGHT],  glm::vec3(0, 0, -1), glm::vec2(0, 0) });
    m_Vertices.push_back({ m_Surfacepoints[BACK_BOTTOM_LEFT],   glm::vec3(0, 0, -1), glm::vec2(1, 0) });
    m_Vertices.push_back({ m_Surfacepoints[BACK_TOP_LEFT],      glm::vec3(0, 0, -1), glm::vec2(1, 1) });
    //Left
    m_Vertices.push_back({ m_Surfacepoints[BACK_TOP_LEFT],      glm::vec3(-1, 0, 0), glm::vec2(0, 1) });
    m_Vertices.push_back({ m_Surfacepoints[BACK_BOTTOM_LEFT],   glm::vec3(-1, 0, 0), glm::vec2(0, 0) });
    m_Vertices.push_back({ m_Surfacepoints[FRONT_BOTTOM_LEFT],  glm::vec3(-1, 0, 0), glm::vec2(1, 0) });
    m_Vertices.push_back({ m_Surfacepoints[FRONT_TOP_LEFT],     glm::vec3(-1, 0, 0), glm::vec2(1, 1) });
    //Right
    m_Vertices.push_back({ m_Surfacepoints[FRONT_TOP_RIGHT],    glm::vec3(1, 0, 0), glm::vec2(0, 1) });
    m_Vertices.push_back({ m_Surfacepoints[FRONT_BOTTOM_RIGHT], glm::vec3(1, 0, 0), glm::vec2(0, 0) });
    m_Vertices.push_back({ m_Surfacepoints[BACK_BOTTOM_RIGHT],  glm::vec3(1, 0, 0), glm::vec2(1, 0) });
    m_Vertices.push_back({ m_Surfacepoints[BACK_TOP_RIGHT],     glm::vec3(1, 0, 0), glm::vec2(1, 1) });
    //Top
    m_Vertices.push_back({ m_Surfacepoints[BACK_TOP_LEFT],   glm::vec3(0, 1, 0), glm::vec2(0, 1) });
    m_Vertices.push_back({ m_Surfacepoints[FRONT_TOP_LEFT],  glm::vec3(0, 1, 0), glm::vec2(0, 0) });
    m_Vertices.push_back({ m_Surfacepoints[FRONT_TOP_RIGHT], glm::vec3(0, 1, 0), glm::vec2(1, 0) });
    m_Vertices.push_back({ m_Surfacepoints[BACK_TOP_RIGHT],  glm::vec3(0, 1, 0), glm::vec2(1, 1) });
    //Bottom
    m_Vertices.push_back({ m_Surfacepoints[FRONT_BOTTOM_LEFT],  glm::vec3(0, -1, 0), glm::vec2(0, 1) });
    m_Vertices.push_back({ m_Surfacepoints[BACK_BOTTOM_LEFT],   glm::vec3(0, -1, 0), glm::vec2(0, 0) });
    m_Vertices.push_back({ m_Surfacepoints[BACK_BOTTOM_RIGHT],  glm::vec3(0, -1, 0), glm::vec2(1, 0) });
    m_Vertices.push_back({ m_Surfacepoints[FRONT_BOTTOM_RIGHT], glm::vec3(0, -1, 0), glm::vec2(1, 1) });
}

void Cube::GenerateIndices()
{
    m_Indices.reserve(36);
    for (unsigned int i = 0; i < 24; i += 4)
    {
        m_Indices.push_back(0 + i);
        m_Indices.push_back(1 + i);
        m_Indices.push_back(2 + i);
        m_Indices.push_back(0 + i);
        m_Indices.push_back(2 + i);
        m_Indices.push_back(3 + i);
    }
}
