#include "Cube.h"

Cube::Cube() :
    Cube(2.f, 2.f, 2.f)
{
}

Cube::Cube(GLfloat width, GLfloat height, GLfloat length) : 
    m_Width(width),
    m_Height(height),
    m_Length(length)
{
    Shape::GenerateShape();
}

Cube::Cube(const json& data)
{
    DeSerialize(data);
    Shape::GenerateShape();
}

json Cube::Serialize() const
{
    json data;
    data["Cube"]["Width"] = m_Width;
    data["Cube"]["Height"] = m_Height;
    data["Cube"]["Length"] = m_Length;
    return data;
}

void Cube::DeSerialize(const json& data)
{
    m_Width = data["Width"];
    m_Height = data["Height"];
    m_Length = data["Length"];
}

Cube* Cube::Clone() const
{
    auto data = Serialize();
    return new Cube(data["Cube"]);
}

void Cube::GenerateVertices()
{
    float x = m_Width / 2;
    float y = m_Height / 2;
    float z = m_Length / 2;

    //Front
    m_Vertices.push_back(Vertex(glm::vec3(-x, y, z), glm::vec2(0, 1)));
    m_Vertices.push_back(Vertex(glm::vec3(-x, -y, z), glm::vec2(0, 0)));
    m_Vertices.push_back(Vertex(glm::vec3(x, -y, z), glm::vec2(1, 0)));
    m_Vertices.push_back(Vertex(glm::vec3(x, y, z), glm::vec2(1, 1)));

    //Back
    m_Vertices.push_back(Vertex(glm::vec3(x, y, -z), glm::vec2(0, 1)));
    m_Vertices.push_back(Vertex(glm::vec3(x, -y, -z), glm::vec2(0, 0)));
    m_Vertices.push_back(Vertex(glm::vec3(-x, -y, -z), glm::vec2(1, 0)));
    m_Vertices.push_back(Vertex(glm::vec3(-x, y, -z), glm::vec2(1, 1)));

    //Left
    m_Vertices.push_back(Vertex(glm::vec3(-x, y, -z), glm::vec2(0, 1)));
    m_Vertices.push_back(Vertex(glm::vec3(-x, -y, -z), glm::vec2(0, 0)));
    m_Vertices.push_back(Vertex(glm::vec3(-x, -y, z), glm::vec2(1, 0)));
    m_Vertices.push_back(Vertex(glm::vec3(-x, y, z), glm::vec2(1, 1)));

    //Right
    m_Vertices.push_back(Vertex(glm::vec3(x, y, z), glm::vec2(0, 1)));
    m_Vertices.push_back(Vertex(glm::vec3(x, -y, z), glm::vec2(0, 0)));
    m_Vertices.push_back(Vertex(glm::vec3(x, -y, -z), glm::vec2(1, 0)));
    m_Vertices.push_back(Vertex(glm::vec3(x, y, -z), glm::vec2(1, 1)));

    //Top
    m_Vertices.push_back(Vertex(glm::vec3(-x, y, -z), glm::vec2(0, 1)));
    m_Vertices.push_back(Vertex(glm::vec3(-x, y, z), glm::vec2(0, 0)));
    m_Vertices.push_back(Vertex(glm::vec3(x, y, z), glm::vec2(1, 0)));
    m_Vertices.push_back(Vertex(glm::vec3(x, y, -z), glm::vec2(1, 1)));

    //Bottom
    m_Vertices.push_back(Vertex(glm::vec3(-x, -y, z), glm::vec2(0, 1)));
    m_Vertices.push_back(Vertex(glm::vec3(-x, -y, -z), glm::vec2(0, 0)));
    m_Vertices.push_back(Vertex(glm::vec3(x, -y, -z), glm::vec2(1, 0)));
    m_Vertices.push_back(Vertex(glm::vec3(x, -y, z), glm::vec2(1, 1)));
}

void Cube::GenerateIndices()
{
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

void Cube::GenerateNormals()
{
    for (unsigned int i = 0; i < 24; i += 4)
    {
        Mesh::GenerateTbn(m_Vertices[0 + i], m_Vertices[1 + i], m_Vertices[2 + i], false);
        Mesh::GenerateTbn(m_Vertices[0 + i], m_Vertices[2 + i], m_Vertices[3 + i], false);
    }
}