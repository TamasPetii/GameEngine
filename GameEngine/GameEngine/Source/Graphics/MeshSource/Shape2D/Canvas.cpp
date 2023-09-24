#include "Canvas.h"

Canvas::Canvas() :
    Canvas(2.f, 2.f)
{
}

Canvas::Canvas(GLfloat width, GLfloat height) :
    m_Width(width),
    m_Height(height)
{
    Shape::GenerateShape();
}

Canvas::Canvas(const json& data)
{
    DeSerialize(data);
    Shape::GenerateShape();
}

json Canvas::Serialize() const
{
    json data;
    data["Canvas"]["Width"] = m_Width;
    data["Canvas"]["Height"] = m_Height;
    return data;
}

void Canvas::DeSerialize(const json& data)
{
    m_Width = data["Width"];
    m_Height = data["Height"];
}

Canvas* Canvas::Clone() const
{
    auto data = Serialize();
    return new Canvas(data["Canvas"]);
}

void Canvas::GenerateVertices()
{
    float x = m_Width / 2;
    float y = m_Height / 2;

    //Top
    m_Vertices.push_back(Vertex(glm::vec3(-x,-y, 0), glm::vec2(0, 1)));
    m_Vertices.push_back(Vertex(glm::vec3(-x, y, 0), glm::vec2(0, 0)));
    m_Vertices.push_back(Vertex(glm::vec3(x, y, 0), glm::vec2(1, 0)));
    m_Vertices.push_back(Vertex(glm::vec3(x, -y, 0), glm::vec2(1, 1)));
}

void Canvas::GenerateIndices()
{
    m_Indices.push_back(0);
    m_Indices.push_back(1);
    m_Indices.push_back(2);
    m_Indices.push_back(0);
    m_Indices.push_back(2);
    m_Indices.push_back(3);
}

void Canvas::GenerateNormals()
{
    Mesh::GenerateTbn(m_Vertices[0], m_Vertices[1], m_Vertices[2], false);
    Mesh::GenerateTbn(m_Vertices[0], m_Vertices[2], m_Vertices[3], false);
}