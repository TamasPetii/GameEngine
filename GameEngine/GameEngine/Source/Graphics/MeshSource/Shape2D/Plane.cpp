#include "Plane.h"

Plane::Plane() :
    Plane(2.f, 2.f)
{
}

Plane::Plane(GLfloat width, GLfloat length) :
    m_Width(width),
    m_Length(length)
{
    Shape::GenerateShape();
}

Plane::Plane(const json& data)
{
    DeSerialize(data);
    Shape::GenerateShape();
}

json Plane::Serialize() const
{
    json data;
    data["Plane"]["Width"] = m_Width;
    data["Plane"]["Length"] = m_Length;
    return data;
}

void Plane::DeSerialize(const json& data)
{
    m_Width = data["Width"];
    m_Length = data["Length"];
}

Plane* Plane::Clone() const
{
    auto data = Serialize();
    return new Plane(data["Plane"]);
}

void Plane::GenerateVertices()
{
    float x = m_Width / 2;
    float z = m_Length / 2;

    //Top
    m_Vertices.push_back(Vertex(glm::vec3(-x, 0, -z), glm::vec2(0, 1)));
    m_Vertices.push_back(Vertex(glm::vec3(-x, 0, z), glm::vec2(0, 0)));
    m_Vertices.push_back(Vertex(glm::vec3(x, 0, z), glm::vec2(1, 0)));
    m_Vertices.push_back(Vertex(glm::vec3(x, 0, -z), glm::vec2(1, 1)));
}

void Plane::GenerateIndices()
{
    m_Indices.push_back(0);
    m_Indices.push_back(1);
    m_Indices.push_back(2);
    m_Indices.push_back(0);
    m_Indices.push_back(2);
    m_Indices.push_back(3);
}

void Plane::GenerateNormals()
{
    Mesh::GenerateTbn(m_Vertices[0], m_Vertices[1], m_Vertices[2], false);
    Mesh::GenerateTbn(m_Vertices[0], m_Vertices[2], m_Vertices[3], false);
}