#include "Plane.h"

Plane* Plane::Clone() const
{
    return new Plane(*this);
}

Plane::Plane(const Plane& other)
    : Shape(other)
{
    m_Name = other.m_Name;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GenerateShape(vertices, indices);
    UploadToGpu(vertices, indices);
}

Plane::Plane()
{
    m_Name = "Plane";
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GenerateShape(vertices, indices);
    UploadToGpu(vertices, indices);
}

void Plane::GenerateShape(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
    //Top
    vertices.push_back(Vertex(glm::vec3(-1, 0, -1), glm::vec3(0, 1, 0), glm::vec2(0, 1)));
    vertices.push_back(Vertex(glm::vec3(-1, 0, 1), glm::vec3(0,1,0), glm::vec2(0, 0)));
    vertices.push_back(Vertex(glm::vec3(1, 0, 1), glm::vec3(0,1,0), glm::vec2(1, 0)));
    vertices.push_back(Vertex(glm::vec3(1, 0, -1), glm::vec3(0,1,0), glm::vec2(1, 1)));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    auto TB = Shape::GenerateTB(vertices[0].normal);

    for (int j = 0; j < 4; j++)
    {
        vertices[j].tangent = TB.first;
        vertices[j].bitangent = TB.second;
    }
}