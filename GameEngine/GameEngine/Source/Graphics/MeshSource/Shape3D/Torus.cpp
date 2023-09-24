#include "Torus.h"

Torus::Torus() :
    Torus(25, 1.f, 0.25f)
{

}

Torus::Torus(GLint count, GLfloat radiusPrimary, GLfloat radiusSecondary) :
    m_Count(count),
    m_RadiusPrimary(radiusPrimary),
    m_RadiusSecondary(radiusSecondary)
{
    Shape::GenerateShape();
}

Torus::Torus(const json& data)
{
    DeSerialize(data);
    Shape::GenerateShape();
}

json Torus::Serialize() const
{
    json data;
    data["Torus"]["Count"] = m_Count;
    data["Torus"]["RadiusPrimary"] = m_RadiusPrimary;
    data["Torus"]["RadiusSecondary"] = m_RadiusSecondary;
    return data;
}

void Torus::DeSerialize(const json& data)
{
    m_Count = data["Count"];
    m_RadiusPrimary = data["RadiusPrimary"];
    m_RadiusSecondary = data["RadiusSecondary"];
}

Torus* Torus::Clone() const
{
    auto data = Serialize();
    return new Torus(data["Torus"]);
}

void Torus::GenerateVertices()
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

            Vertex vertex;
            vertex.position = glm::vec3(x, y, z);
            vertex.texture = glm::vec2(u, 1 - v);
            m_Vertices.push_back(vertex);
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

void Torus::GenerateNormals()
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