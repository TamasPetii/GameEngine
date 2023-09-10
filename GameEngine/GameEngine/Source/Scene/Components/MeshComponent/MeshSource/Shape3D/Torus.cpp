#include "Torus.h"

Torus* Torus::Clone() const
{
    return new Torus(*this);
}

Torus::Torus(const Torus& other)
    : Shape(other)
{
    std::cout << "Torus Copy Constructor!" << std::endl;

    m_Name = other.m_Name;
    m_Count = other.m_Count;
    m_RadiusPrimary = other.m_RadiusPrimary;
    m_RadiusSecondary = other.m_RadiusSecondary;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GenerateShape(vertices, indices);
    UploadToGpu(vertices, indices);
}

Torus::Torus()
{
    m_Name = "Torus";
    m_Count = 50;
    m_RadiusPrimary = 1.f;
    m_RadiusSecondary = 0.25f;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GenerateShape(vertices, indices);
    UploadToGpu(vertices, indices);
}

void Torus::GenerateShape(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
    for (unsigned int h = 0; h <= m_Count; h++)
    {
        for (unsigned int w = 0; w <= m_Count; w++)
        {
            float u = w / static_cast<float>(m_Count);
            float v = h / static_cast<float>(m_Count);
            float theta = 2 * M_PI * u;
            float phi = 2 * M_PI * v;

            float x = (m_RadiusPrimary + m_RadiusSecondary * cosf(phi)) * cosf(theta);
            float y = m_RadiusSecondary * sinf(phi);
            float z = (m_RadiusPrimary + m_RadiusSecondary * cosf(phi)) * sinf(theta);

            auto normal = glm::vec3(cos(theta) * cosf(phi), sinf(phi), sinf(theta) * cosf(phi));
            auto TB = Shape::GenerateTB(normal);

            Vertex vertex;
            vertex.position = glm::vec3(x, y, z);
            vertex.normal = glm::normalize(normal);
            vertex.tangent = glm::normalize(TB.first);
            vertex.bitangent = glm::normalize(TB.second);
            vertex.texture = glm::vec2(1 - u, 1 - v);
            vertices.push_back(vertex);
        }
    }

    for (unsigned int h = 0; h < m_Count; h++)
    {
        for (unsigned int w = 0; w < m_Count; w++)
        {
            int c = w + h * (m_Count + 1);
            indices.push_back(c);
            indices.push_back(c + m_Count + 1);
            indices.push_back(c + m_Count + 2);
            indices.push_back(c);
            indices.push_back(c + m_Count + 2);
            indices.push_back(c + 1);
        }
    }
}