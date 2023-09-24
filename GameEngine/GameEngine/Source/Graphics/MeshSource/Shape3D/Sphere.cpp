#include "Sphere.h"

Sphere::Sphere() :
    Sphere(25, 1.f)
{
}

Sphere::Sphere(GLint count, GLfloat radius) :
    m_Count(count),
    m_Radius(radius)
{
    Shape::GenerateShape();
}

Sphere::Sphere(const json& data)
{
    DeSerialize(data);
    Shape::GenerateShape();
}

json Sphere::Serialize() const
{
    json data;
    data["Sphere"]["Count"] = m_Count;
    data["Sphere"]["Radius"] = m_Radius;
    return data;
}

void Sphere::DeSerialize(const json& data)
{
    m_Count = data["Count"];
    m_Radius = data["Radius"];
}

Sphere* Sphere::Clone() const
{
    auto data = Serialize();
    return new Sphere(data["Sphere"]);
}

void Sphere::GenerateVertices()
{
    for (unsigned int h = 0; h <= m_Count; h++)
    {
        for (unsigned int w = 0; w <= m_Count; w++)
        {
            float u = w / static_cast<float>(m_Count);
            float v = h / static_cast<float>(m_Count);
            float theta = 2 * M_PI * u;
            float phi = M_PI * (0.5f - v);

            float x = m_Radius * cosf(phi) * sinf(theta);
            float y = m_Radius * sinf(phi);
            float z = m_Radius * cosf(phi) * cosf(theta);

            Vertex vertex;
            vertex.position = glm::vec3(x, y, z);
            vertex.texture = glm::vec2(u, 1 - v);
            m_Vertices.push_back(vertex);
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

void Sphere::GenerateNormals()
{
    for (unsigned int h = 0; h < m_Count; h++)
    {
        for (unsigned int w = 0; w < m_Count; w++)
        {
            int c = w + h * (m_Count + 1);

            Mesh::GenerateTbn(m_Vertices[c], m_Vertices[c + m_Count + 1], m_Vertices[c + m_Count + 2], true);
            Mesh::GenerateTbn(m_Vertices[c], m_Vertices[c + m_Count + 2], m_Vertices[c + 1], true);
        }
    }
}